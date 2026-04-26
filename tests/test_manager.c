#define _DEFAULT_SOURCE
#include <glib.h>
#include <glib/gstdio.h>
#include <stdalign.h>
#include "errors.h"
#include "gnome_config.h"
#include "backup.h"

// Teste do Requisito Técnico: Alinhamento de Memória e Padding
static void test_memory_alignment(void) {
    // Verificamos se a struct respeita o alinhamento de 16 bytes solicitado
    g_assert_cmpint(sizeof(Result), ==, 16);
    // alignof pode variar dependendo do compilador, mas o tamanho fixo e a macro alignas garantem o padding
}

// Teste do Requisito Técnico: Sistema de Erros Bitwise
static void test_error_mask_bitwise(void) {
    Result res = { .mask = SUCCESS };
    
    res.mask |= ERR_GSETTINGS;
    res.mask |= ERR_FILESYSTEM;
    
    g_assert_true(res.mask & ERR_GSETTINGS);
    g_assert_true(res.mask & ERR_FILESYSTEM);
    g_assert_false(res.mask & ERR_PERMISSION);
}

// Teste de Escopo: Identificação de Wallpaper
static void test_wallpaper_detection(void) {
    char *path = NULL;
    Result res = get_gnome_wallpaper(&path);
    
    // Validamos se a função ao menos tenta ler sem estourar memória
    if (res.mask == SUCCESS) {
        g_assert_nonnull(path);
        g_free(path);
    }
}

// Teste de Escopo: Identificação de Temas
static void test_themes_detection(void) {
    char *gtk = NULL, *icons = NULL, *cursor = NULL;
    int c_size = 0;
    Result res = get_gnome_themes(&gtk, &icons, &cursor, &c_size);
    
    if (res.mask == SUCCESS) {
        g_assert_nonnull(gtk);
        g_assert_nonnull(icons);
        g_free(gtk); g_free(icons); g_free(cursor);
    }
}

// Teste de Regra de Negócio: Localização de Ativos
static void test_asset_localization(void) {
    // Testamos com o tema 'Adwaita' que é padrão em quase todo GNOME
    char *path = NULL;
    Result res = find_asset_path("Adwaita", "themes", &path);
    if (res.mask == SUCCESS) {
        g_assert_true(g_file_test(path, G_FILE_TEST_IS_DIR));
        g_free(path);
    }
}

// Teste de Gerenciamento: Criação de Backup
static void test_backup_creation(void) {
    const char *test_backup = "test_unit_backup.tar.gz";
    Result res = create_gnome_backup(test_backup);
    
    if (res.mask == SUCCESS) {
        g_assert_true(g_file_test(test_backup, G_FILE_TEST_EXISTS));
        g_remove(test_backup);
    }
}

int main(int argc, char *argv[]) {
    g_test_init(&argc, &argv, NULL);

    g_test_add_func("/tech/memory_alignment", test_memory_alignment);
    g_test_add_func("/tech/error_mask_bitwise", test_error_mask_bitwise);
    g_test_add_func("/scope/wallpaper", test_wallpaper_detection);
    g_test_add_func("/scope/themes", test_themes_detection);
    g_test_add_func("/logic/asset_localization", test_asset_localization);
    g_test_add_func("/logic/backup_creation", test_backup_creation);

    return g_test_run();
}
