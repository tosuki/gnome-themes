#ifndef TUI_H
#define TUI_H

#include "errors.h"

/**
 * @brief Inicializa a interface TUI.
 */
void init_tui(void);

/**
 * @brief Finaliza a interface TUI.
 */
void close_tui(void);

/**
 * @brief Exibe o menu principal e gerencia a navegação.
 * 
 * @return int A opção selecionada.
 */
int run_main_menu(void);

/**
 * @brief Exibe o dashboard com a configuração atual.
 */
void show_dashboard(void);

/**
 * @brief Abre um explorador de arquivos simples para selecionar um backup.
 * 
 * @return char* Caminho do arquivo selecionado (deve ser liberado com free).
 */
char* run_file_explorer(void);

#endif // TUI_H
