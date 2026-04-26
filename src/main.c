#include <stdio.h>
#include <glib.h>
#include <gio/gio.h>
#include "errors.h"
#include "gnome_config.h"
#include "backup.h"

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    printf("Gnome Themes Manager inicializado.\n");

    // ... detecções anteriores omitidas para brevidade no teste de backup ...
    printf("Criando backup de teste em 'backup.tar.gz'...\n");
    Result res = create_gnome_backup("backup.tar.gz");
    
    if (res.mask == SUCCESS) {
        printf("Backup criado com sucesso!\n");
    } else {
        printf("Erro ao criar backup: Mask 0x%X\n", res.mask);
    }

    return 0;
}
