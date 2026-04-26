#ifndef KEYBINDINGS_H
#define KEYBINDINGS_H

#include <glib.h>
#include "errors.h"

/**
 * @brief Salva todas as keybindings (sistema e personalizadas) em um GKeyFile.
 * 
 * @param keyfile O GKeyFile onde os dados serão salvos.
 * @return Result 
 */
Result export_gnome_keybindings(GKeyFile *keyfile);

/**
 * @brief Aplica as keybindings de um GKeyFile ao sistema.
 * 
 * @param keyfile O GKeyFile contendo os dados.
 * @return Result 
 */
Result import_gnome_keybindings(GKeyFile *keyfile);

#endif // KEYBINDINGS_H
