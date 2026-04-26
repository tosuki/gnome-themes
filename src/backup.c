#define _DEFAULT_SOURCE
#include <archive.h>
#include <archive_entry.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <gio/gio.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include "backup.h"
#include "gnome_config.h"

static Result add_file_to_archive(struct archive *a, const char *path, const char *name_in_archive) {
    Result res = { .timestamp = (uint64_t)g_get_real_time(), .mask = SUCCESS };
    struct stat st;
    if (stat(path, &st) != 0) {
        res.mask |= ERR_FILESYSTEM;
        return res;
    }

    struct archive_entry *entry = archive_entry_new();
    archive_entry_set_pathname(entry, name_in_archive);
    archive_entry_set_size(entry, st.st_size);
    archive_entry_set_filetype(entry, AE_IFREG);
    archive_entry_set_perm(entry, st.st_mode);
    archive_write_header(a, entry);

    int fd = open(path, O_RDONLY);
    if (fd >= 0) {
        char buff[16384];
        ssize_t len = read(fd, buff, sizeof(buff));
        while (len > 0) {
            archive_write_data(a, buff, len);
            len = read(fd, buff, sizeof(buff));
        }
        close(fd);
    } else {
        res.mask |= ERR_PERMISSION;
    }

    archive_entry_free(entry);
    return res;
}

static Result add_dir_recursive(struct archive *a, const char *path, const char *base_name) {
    Result res = { .timestamp = (uint64_t)g_get_real_time(), .mask = SUCCESS };
    DIR *dir = opendir(path);
    if (!dir) return res;

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;

        char *full_path = g_build_filename(path, entry->d_name, NULL);
        char *archive_path = g_build_filename(base_name, entry->d_name, NULL);

        if (entry->d_type == DT_DIR) {
            add_dir_recursive(a, full_path, archive_path);
        } else if (entry->d_type == DT_REG) {
            add_file_to_archive(a, full_path, archive_path);
        }

        g_free(full_path);
        g_free(archive_path);
    }

    closedir(dir);
    return res;
}

Result create_gnome_backup(const char *output_file) {
    Result res = { .timestamp = (uint64_t)g_get_real_time(), .mask = SUCCESS };
    struct archive *a = archive_write_new();
    archive_write_add_filter_gzip(a);
    archive_write_set_format_pax_restricted(a);
    
    if (archive_write_open_filename(a, output_file) != ARCHIVE_OK) {
        res.mask |= ERR_PERMISSION;
        archive_write_free(a);
        return res;
    }

    char *gtk = NULL, *icons = NULL, *cursor = NULL, *wallpaper = NULL;
    int cursor_size = 0;
    get_gnome_themes(&gtk, &icons, &cursor, &cursor_size);
    get_gnome_wallpaper(&wallpaper);

    char *config_data = g_strdup_printf("gtk=%s\nicons=%s\ncursor=%s\ncursor_size=%d\nwallpaper=%s\n", 
                                        gtk ? gtk : "", icons ? icons : "", cursor ? cursor : "", 
                                        cursor_size, wallpaper ? wallpaper : "");
    
    struct archive_entry *entry = archive_entry_new();
    archive_entry_set_pathname(entry, "manifest.conf");
    archive_entry_set_size(entry, strlen(config_data));
    archive_entry_set_filetype(entry, AE_IFREG);
    archive_entry_set_perm(entry, 0644);
    archive_write_header(a, entry);
    archive_write_data(a, config_data, strlen(config_data));
    archive_entry_free(entry);
    g_free(config_data);

    char *path = NULL;
    if (gtk && find_asset_path(gtk, "themes", &path).mask == SUCCESS) {
        add_dir_recursive(a, path, "themes");
        g_free(path);
    }
    if (icons && find_asset_path(icons, "icons", &path).mask == SUCCESS) {
        add_dir_recursive(a, path, "icons");
        g_free(path);
    }
    if (cursor && find_asset_path(cursor, "icons", &path).mask == SUCCESS) {
        add_dir_recursive(a, path, "cursor");
        g_free(path);
    }

    if (wallpaper && g_str_has_prefix(wallpaper, "file://")) {
        char *w_path = g_filename_from_uri(wallpaper, NULL, NULL);
        if (w_path) {
            add_file_to_archive(a, w_path, "wallpaper.img");
            g_free(w_path);
        }
    }

    g_free(gtk); g_free(icons); g_free(cursor); g_free(wallpaper);
    archive_write_close(a);
    archive_write_free(a);
    return res;
}
