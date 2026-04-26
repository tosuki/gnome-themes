#include <gio/gio.h>
#include "gnome_config.h"

Result get_gnome_wallpaper(char **path) {
    Result res = { .timestamp = (uint64_t)g_get_real_time(), .mask = SUCCESS, .reserved = 0 };
    
    GSettings *settings = g_settings_new("org.gnome.desktop.background");
    if (!settings) {
        res.mask |= ERR_GSETTINGS;
        return res;
    }

    *path = g_settings_get_string(settings, "picture-uri");
    
    // O GNOME costuma retornar com o prefixo 'file://', vamos tratar isso se necessário no futuro
    if (*path == NULL || strlen(*path) == 0) {
        res.mask |= ERR_THEME_NOT_FOUND;
    }

    g_object_unref(settings);
    return res;
}
