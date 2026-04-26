#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include "tui.h"
#include "gnome_config.h"

void init_tui(void) {
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);
    start_color();
    
    init_pair(1, COLOR_CYAN, COLOR_BLACK);   // Títulos
    init_pair(2, COLOR_GREEN, COLOR_BLACK);  // Seleção
    init_pair(3, COLOR_WHITE, COLOR_BLACK);  // Texto normal
    init_pair(4, COLOR_RED, COLOR_BLACK);    // Erros
    init_pair(5, COLOR_YELLOW, COLOR_BLACK); // Diretórios
}

void close_tui(void) {
    endwin();
}

static void draw_menu(int highlight, const char *choices[], int n_choices) {
    int x = 2, y = 2;
    attron(COLOR_PAIR(1) | A_BOLD);
    mvprintw(y++, x, "=== Gnome Themes Manager ===");
    attroff(COLOR_PAIR(1) | A_BOLD);
    y++;

    for (int i = 0; i < n_choices; i++) {
        if (highlight == i + 1) {
            attron(COLOR_PAIR(2) | A_REVERSE);
            mvprintw(y, x, " > %s ", choices[i]);
            attroff(COLOR_PAIR(2) | A_REVERSE);
        } else {
            mvprintw(y, x, "   %s ", choices[i]);
        }
        y++;
    }
    
    mvprintw(LINES - 2, x, "Use as setas para navegar e Enter para selecionar.");
    refresh();
}

int run_main_menu(void) {
    const char *choices[] = {
        "Visualizar Configuração Atual",
        "Salvar Configuração (Backup)",
        "Restaurar Configuração",
        "Sair"
    };
    int n_choices = sizeof(choices) / sizeof(char *);
    int highlight = 1;
    int c;

    while (1) {
        clear();
        draw_menu(highlight, choices, n_choices);
        c = getch();
        switch (c) {
            case KEY_UP:
                if (highlight == 1) highlight = n_choices;
                else highlight--;
                break;
            case KEY_DOWN:
                if (highlight == n_choices) highlight = 1;
                else highlight++;
                break;
            case 10: // Enter
                return highlight;
        }
    }
}

void show_dashboard(void) {
    clear();
    char *gtk = NULL, *icons = NULL, *cursor = NULL, *wallpaper = NULL;
    int c_size = 0;
    get_gnome_themes(&gtk, &icons, &cursor, &c_size);
    get_gnome_wallpaper(&wallpaper);

    attron(COLOR_PAIR(1) | A_BOLD);
    mvprintw(2, 2, "--- Dashboard de Configuração ---");
    attroff(COLOR_PAIR(1) | A_BOLD);

    attron(COLOR_PAIR(3));
    mvprintw(4, 4, "Wallpaper:  %s", wallpaper ? wallpaper : "N/A");
    mvprintw(5, 4, "Tema GTK:   %s", gtk ? gtk : "N/A");
    mvprintw(6, 4, "Ícones:     %s", icons ? icons : "N/A");
    mvprintw(7, 4, "Cursor:     %s (tamanho %d)", cursor ? cursor : "N/A", c_size);
    attroff(COLOR_PAIR(3));

    mvprintw(10, 2, "Pressione qualquer tecla para voltar...");
    
    g_free(gtk); g_free(icons); g_free(cursor); g_free(wallpaper);
    refresh();
    getch();
}

typedef struct {
    char *name;
    bool is_dir;
} FileEntry;

static int compare_entries(const void *a, const void *b) {
    FileEntry *ea = (FileEntry *)a;
    FileEntry *eb = (FileEntry *)b;
    if (ea->is_dir && !eb->is_dir) return -1;
    if (!ea->is_dir && eb->is_dir) return 1;
    return strcmp(ea->name, eb->name);
}

char* run_file_explorer(void) {
    char current_path[PATH_MAX];
    getcwd(current_path, sizeof(current_path));
    
    while (1) {
        DIR *dir = opendir(current_path);
        if (!dir) return NULL;

        FileEntry entries[256];
        int n_entries = 0;
        
        // Adicionar opção de voltar
        entries[n_entries].name = strdup("..");
        entries[n_entries].is_dir = true;
        n_entries++;

        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL && n_entries < 256) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
            
            // Filtro: apenas pastas ou arquivos .tar.gz
            bool is_dir = (entry->d_type == DT_DIR);
            if (is_dir || g_str_has_suffix(entry->d_name, ".tar.gz")) {
                entries[n_entries].name = strdup(entry->d_name);
                entries[n_entries].is_dir = is_dir;
                n_entries++;
            }
        }
        closedir(dir);
        qsort(entries + 1, n_entries - 1, sizeof(FileEntry), compare_entries);

        int highlight = 1;
        bool selected = false;
        while (!selected) {
            clear();
            attron(COLOR_PAIR(1) | A_BOLD);
            mvprintw(1, 2, "Explorador de Arquivos: %s", current_path);
            attroff(COLOR_PAIR(1) | A_BOLD);
            
            for (int i = 0; i < n_entries; i++) {
                if (highlight == i + 1) attron(A_REVERSE);
                if (entries[i].is_dir) attron(COLOR_PAIR(5));
                mvprintw(i + 3, 4, "%s %s", entries[i].is_dir ? "[DIR]" : "     ", entries[i].name);
                attroff(COLOR_PAIR(5) | A_REVERSE);
            }
            
            mvprintw(LINES - 2, 2, "Enter: Selecionar/Abrir | ESC: Cancelar");
            refresh();

            int c = getch();
            switch (c) {
                case KEY_UP: highlight = (highlight == 1) ? n_entries : highlight - 1; break;
                case KEY_DOWN: highlight = (highlight == n_entries) ? 1 : highlight + 1; break;
                case 27: // ESC
                    for (int i = 0; i < n_entries; i++) free(entries[i].name);
                    return NULL;
                case 10: // Enter
                    if (entries[highlight - 1].is_dir) {
                        char *new_path = g_build_filename(current_path, entries[highlight - 1].name, NULL);
                        char *real_path = realpath(new_path, NULL);
                        if (real_path) {
                            strcpy(current_path, real_path);
                            free(real_path);
                        }
                        g_free(new_path);
                        selected = true; // Sair do loop interno para recarregar diretório
                    } else {
                        char *result = g_build_filename(current_path, entries[highlight - 1].name, NULL);
                        for (int i = 0; i < n_entries; i++) free(entries[i].name);
                        return result;
                    }
                    break;
            }
        }
        for (int i = 0; i < n_entries; i++) free(entries[i].name);
    }
}
