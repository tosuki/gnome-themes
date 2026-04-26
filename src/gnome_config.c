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

Result get_gnome_fonts(char **interface, char **document, char **monospace, char **title) {
    Result res = { .timestamp = (uint64_t)g_get_real_time(), .mask = SUCCESS, .reserved = 0 };
    GSettings *desktop_settings = g_settings_new("org.gnome.desktop.interface");
    
    if (interface) *interface = g_settings_get_string(desktop_settings, "font-name");
    if (document) *document = g_settings_get_string(desktop_settings, "document-font-name");
    if (monospace) *monospace = g_settings_get_string(desktop_settings, "monospace-font-name");
    
    g_object_unref(desktop_settings);
    
    GSettings *wm_settings = g_settings_new("org.gnome.desktop.wm.preferences");
    if (title) *title = g_settings_get_string(wm_settings, "titlebar-font");
    g_object_unref(wm_settings);
    
    return res;
}

Result get_gnome_preferences(char **color_scheme) {
    Result res = { .timestamp = (uint64_t)g_get_real_time(), .mask = SUCCESS, .reserved = 0 };
    GSettings *settings = g_settings_new("org.gnome.desktop.interface");
    
    if (color_scheme) *color_scheme = g_settings_get_string(settings, "color-scheme");
    
    g_object_unref(settings);
    return res;
}
