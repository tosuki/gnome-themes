#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <ncurses.h>
#include "tui.h"
#include "backup.h"

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    init_tui();

    int choice;
    while ((choice = run_main_menu()) != 4) {
        switch (choice) {
            case 1:
                show_dashboard();
                break;
            case 2:
                {
                    char *filename = g_strdup_printf("backup_%ld.tar.gz", (long)g_get_real_time());
                    create_gnome_backup(filename);
                    clear();
                    mvprintw(2, 2, "Backup salvo como: %s", filename);
                    mvprintw(4, 2, "Pressione qualquer tecla...");
                    refresh();
                    getch();
                    g_free(filename);
                }
                break;
            case 3:
                {
                    char *backup_file = run_file_explorer();
                    if (backup_file) {
                        // Criar ponto de restauração automático
                        char *rp_name = g_strdup_printf("restore_point_%ld.tar.gz", (long)g_get_real_time());
                        create_gnome_backup(rp_name);
                        
                        Result res = restore_gnome_backup(backup_file);
                        clear();
                        if (res.mask == SUCCESS) {
                            mvprintw(2, 2, "Configuração restaurada com sucesso!");
                            mvprintw(3, 2, "Ponto de restauração criado: %s", rp_name);
                        } else {
                            attron(COLOR_PAIR(4));
                            mvprintw(2, 2, "Erro ao restaurar (Mask: 0x%X)", (unsigned int)res.mask);
                            attroff(COLOR_PAIR(4));
                        }
                        g_free(rp_name);
                        mvprintw(5, 2, "Pressione qualquer tecla...");
                        refresh();
                        getch();
                        free(backup_file);
                    }
                }
                break;
            default:
                break;
        }
    }

    close_tui();
    return 0;
}
