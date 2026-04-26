#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include "tui.h"
#include "gnome_config.h"

void init_tui(void) {
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);
    start_color();
    
    // Definir pares de cores
    init_pair(1, COLOR_CYAN, COLOR_BLACK);   // Títulos
    init_pair(2, COLOR_GREEN, COLOR_BLACK);  // Seleção
    init_pair(3, COLOR_WHITE, COLOR_BLACK);  // Texto normal
    init_pair(4, COLOR_RED, COLOR_BLACK);    // Erros
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
    int choice = 0;
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
                choice = highlight;
                return choice;
            default:
                break;
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

char* run_file_explorer(void) {
    // Versão simplificada por enquanto
    clear();
    mvprintw(2, 2, "Digite o nome do arquivo de backup para restaurar:");
    mvprintw(3, 2, "(Pressione ESC para cancelar)");
    
    echo();
    curs_set(1);
    char *input = malloc(256);
    mvgetnstr(4, 2, input, 255);
    noecho();
    curs_set(0);
    
    if (strlen(input) == 0) {
        free(input);
        return NULL;
    }
    return input;
}
