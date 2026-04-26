#ifndef GNOME_CONFIG_H
#define GNOME_CONFIG_H

#include <glib.h>
#include "errors.h"

/**
 * @brief Obtém o caminho do wallpaper atual do GNOME.
 * 
 * @param path Ponteiro onde o caminho será armazenado (deve ser liberado com g_free).
 * @return Result Estrutura contendo o status da operação.
 */
Result get_gnome_wallpaper(char **path);

/**
 * @brief Obtém os temas atuais (GTK, ícones, cursor).
 * 
 * @param gtk_theme Ponteiro para o nome do tema GTK.
 * @param icon_theme Ponteiro para o nome do tema de ícones.
 * @param cursor_theme Ponteiro para o nome do tema do cursor.
 * @param cursor_size Ponteiro para o tamanho do cursor.
 * @return Result 
 */
Result get_gnome_themes(char **gtk_theme, char **icon_theme, char **cursor_theme, int *cursor_size);

#endif // GNOME_CONFIG_H
