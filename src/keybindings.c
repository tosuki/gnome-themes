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

    // 1. Exportar atalhos de sistema
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

    // 2. Exportar atalhos personalizados
    GSettings *media_keys = g_settings_new("org.gnome.settings-daemon.plugins.media-keys");
    char **custom_paths = g_settings_get_strv(media_keys, "custom-keybindings");
    
    if (custom_paths) {
        g_key_file_set_string_list(keyfile, "custom", "paths", (const char * const *)custom_paths, g_strv_length(custom_paths));
        
        for (int i = 0; custom_paths[i] != NULL; i++) {
            // O caminho é algo como /org/gnome/settings-daemon/plugins/media-keys/custom-keybindings/custom0/
            // Precisamos criar um GSettings baseado no caminho, não no esquema puro
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
    
    // Implementação simétrica ao exportar será feita após validar o export
    // ...
    
    return res;
}
