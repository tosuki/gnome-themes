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

Result get_gnome_themes(char **gtk_theme, char **icon_theme, char **cursor_theme, int *cursor_size) {
    Result res = { .timestamp = (uint64_t)g_get_real_time(), .mask = SUCCESS, .reserved = 0 };
    
    GSettings *interface_settings = g_settings_new("org.gnome.desktop.interface");
    if (!interface_settings) {
        res.mask |= ERR_GSETTINGS;
        return res;
    }

    if (gtk_theme) *gtk_theme = g_settings_get_string(interface_settings, "gtk-theme");
    if (icon_theme) *icon_theme = g_settings_get_string(interface_settings, "icon-theme");
    if (cursor_theme) *cursor_theme = g_settings_get_string(interface_settings, "cursor-theme");
    if (cursor_size) *cursor_size = g_settings_get_int(interface_settings, "cursor-size");

    g_object_unref(interface_settings);
    return res;
}
