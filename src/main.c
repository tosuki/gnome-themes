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

    return 0;
}
