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
        printf("Tema Ícones: %s\n", icons);
        printf("Tema Cursor: %s (%d)\n", cursor, cursor_size);
        g_free(gtk);
        g_free(icons);
        g_free(cursor);
    } else {
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
