#include <gio/gio.h>
#include <string.h>
#include "keybindings.h"

static const char *system_schemas[] = {
    "org.gnome.desktop.wm.keybindings",
    "org.gnome.settings-daemon.plugins.media-keys",
    "org.gnome.shell.keybindings",
    NULL
};

Result export_gnome_keybindings(GKeyFile *keyfile) {
    Result res = { .timestamp = (uint64_t)g_get_real_time(), .mask = SUCCESS };

    for (int i = 0; system_schemas[i] != NULL; i++) {
        GSettings *settings = g_settings_new(system_schemas[i]);
        char **keys = g_settings_list_keys(settings);
        
        for (int j = 0; keys[j] != NULL; j++) {
            GVariant *val = g_settings_get_value(settings, keys[j]);
            char *str_val = g_variant_print(val, FALSE);
            g_key_file_set_string(keyfile, system_schemas[i], keys[j], str_val);
            g_free(str_val);
            g_variant_unref(val);
        }
        
        g_strfreev(keys);
        g_object_unref(settings);
    }

    GSettings *media_keys = g_settings_new("org.gnome.settings-daemon.plugins.media-keys");
    char **custom_paths = g_settings_get_strv(media_keys, "custom-keybindings");
    
    if (custom_paths && g_strv_length(custom_paths) > 0) {
        g_key_file_set_string_list(keyfile, "custom", "paths", (const char * const *)custom_paths, g_strv_length(custom_paths));
        
        for (int i = 0; custom_paths[i] != NULL; i++) {
            GSettings *custom = g_settings_new_with_path("org.gnome.settings-daemon.plugins.media-keys.custom-keybinding", custom_paths[i]);
            
            char *name = g_settings_get_string(custom, "name");
            char *command = g_settings_get_string(custom, "command");
            char *binding = g_settings_get_string(custom, "binding");
            
            g_key_file_set_string(keyfile, custom_paths[i], "name", name);
            g_key_file_set_string(keyfile, custom_paths[i], "command", command);
            g_key_file_set_string(keyfile, custom_paths[i], "binding", binding);
            
            g_free(name); g_free(command); g_free(binding);
            g_object_unref(custom);
        }
        g_strfreev(custom_paths);
    }
    g_object_unref(media_keys);

    return res;
}

Result import_gnome_keybindings(GKeyFile *keyfile) {
    Result res = { .timestamp = (uint64_t)g_get_real_time(), .mask = SUCCESS };

    // 1. Restaurar atalhos de sistema
    for (int i = 0; system_schemas[i] != NULL; i++) {
        if (!g_key_file_has_group(keyfile, system_schemas[i])) continue;
        
        GSettings *settings = g_settings_new(system_schemas[i]);
        gsize n_keys;
        char **keys = g_key_file_get_keys(keyfile, system_schemas[i], &n_keys, NULL);
        
        for (size_t j = 0; j < n_keys; j++) {
            char *val_str = g_key_file_get_string(keyfile, system_schemas[i], keys[j], NULL);
            GVariant *v = g_variant_parse(NULL, val_str, NULL, NULL, NULL);
            if (v) {
                g_settings_set_value(settings, keys[j], v);
                g_variant_unref(v);
            }
            g_free(val_str);
        }
        g_strfreev(keys);
        g_object_unref(settings);
    }

    // 2. Restaurar atalhos personalizados
    if (g_key_file_has_group(keyfile, "custom")) {
        gsize n_paths;
        char **paths = g_key_file_get_string_list(keyfile, "custom", "paths", &n_paths, NULL);
        
        GSettings *media_keys = g_settings_new("org.gnome.settings-daemon.plugins.media-keys");
        g_settings_set_strv(media_keys, "custom-keybindings", (const char * const *)paths);
        
        for (size_t i = 0; i < n_paths; i++) {
            if (!g_key_file_has_group(keyfile, paths[i])) continue;
            
            GSettings *custom = g_settings_new_with_path("org.gnome.settings-daemon.plugins.media-keys.custom-keybinding", paths[i]);
            
            char *name = g_key_file_get_string(keyfile, paths[i], "name", NULL);
            char *command = g_key_file_get_string(keyfile, paths[i], "command", NULL);
            char *binding = g_key_file_get_string(keyfile, paths[i], "binding", NULL);
            
            if (name) g_settings_set_string(custom, "name", name);
            if (command) g_settings_set_string(custom, "command", command);
            if (binding) g_settings_set_string(custom, "binding", binding);
            
            g_free(name); g_free(command); g_free(binding);
            g_object_unref(custom);
        }
        
        g_strfreev(paths);
        g_object_unref(media_keys);
    }

    return res;
}
