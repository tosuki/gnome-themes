#include <archive.h>
#include <archive_entry.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <gio/gio.h>
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
    archive_entry_set_perm(entry, 0644);
    archive_write_header(a, entry);

    int fd = open(path, O_RDONLY);
    if (fd >= 0) {
        char buff[8192];
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

    // Por enquanto, vamos apenas salvar os nomes das configurações em um arquivo "config.txt" dentro do tar
    char *gtk = NULL, *icons = NULL, *cursor = NULL;
    get_gnome_themes(&gtk, &icons, &cursor, NULL);
    
    char *config_data = g_strdup_printf("gtk-theme=%s\nicon-theme=%s\ncursor-theme=%s\n", 
                                        gtk ? gtk : "", icons ? icons : "", cursor ? cursor : "");
    
    struct archive_entry *entry = archive_entry_new();
    archive_entry_set_pathname(entry, "config.txt");
    archive_entry_set_size(entry, strlen(config_data));
    archive_entry_set_filetype(entry, AE_IFREG);
    archive_entry_set_perm(entry, 0644);
    archive_write_header(a, entry);
    archive_write_data(a, config_data, strlen(config_data));
    archive_entry_free(entry);

    g_free(config_data);
    g_free(gtk); g_free(icons); g_free(cursor);

    archive_write_close(a);
    archive_write_free(a);
    return res;
}
