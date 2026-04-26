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

#endif // GNOME_CONFIG_H
