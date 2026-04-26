#include <stdio.h>
#include <glib.h>
#include <gio/gio.h>
#include "errors.h"
#include "gnome_config.h"

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    printf("Gnome Themes Manager inicializado.\n");

    char *wallpaper_path = NULL;
    Result res = get_gnome_wallpaper(&wallpaper_path);

    if (res.mask == SUCCESS) {
        printf("Wallpaper atual: %s\n", wallpaper_path);
        g_free(wallpaper_path);
    } else {
        printf("Erro ao detectar wallpaper: Mask 0x%X\n", res.mask);
    }

    char *gtk = NULL, *icons = NULL, *cursor = NULL;
    int cursor_size = 0;
    res = get_gnome_themes(&gtk, &icons, &cursor, &cursor_size);

    if (res.mask == SUCCESS) {
        printf("Tema GTK: %s\n", gtk);
        char *gtk_path = NULL;
        if (find_asset_path(gtk, "themes", &gtk_path).mask == SUCCESS) {
            printf("  - Caminho: %s\n", gtk_path);
            g_free(gtk_path);
        }

        printf("Tema Ícones: %s\n", icons);
        char *icon_path = NULL;
        if (find_asset_path(icons, "icons", &icon_path).mask == SUCCESS) {
            printf("  - Caminho: %s\n", icon_path);
            g_free(icon_path);
        }

        printf("Tema Cursor: %s (%d)\n", cursor, cursor_size);
        char *cursor_path = NULL;
        if (find_asset_path(cursor, "icons", &cursor_path).mask == SUCCESS) {
            printf("  - Caminho: %s\n", cursor_path);
            g_free(cursor_path);
        }

        g_free(gtk);
        g_free(icons);
        g_free(cursor);
    }
 else {
        printf("Erro ao detectar temas: Mask 0x%X\n", res.mask);
    }

    char *f_iface = NULL, *f_doc = NULL, *f_mono = NULL, *f_title = NULL;
    res = get_gnome_fonts(&f_iface, &f_doc, &f_mono, &f_title);
    if (res.mask == SUCCESS) {
        printf("Fontes: Interface(%s), Doc(%s), Mono(%s), Title(%s)\n", f_iface, f_doc, f_mono, f_title);
        g_free(f_iface); g_free(f_doc); g_free(f_mono); g_free(f_title);
    }

    char *scheme = NULL;
    res = get_gnome_preferences(&scheme);
    if (res.mask == SUCCESS) {
        printf("Esquema de cores: %s\n", scheme);
        g_free(scheme);
    }

    return 0;
}
