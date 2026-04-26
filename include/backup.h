#ifndef BACKUP_H
#define BACKUP_H

#include <glib.h>
#include "errors.h"

/**
 * @brief Cria um backup comprimido das configurações e temas atuais.
 * 
 * @param output_file Caminho do arquivo .tar.gz de saída.
 * @return Result 
 */
Result create_gnome_backup(const char *output_file);

/**
 * @brief Restaura uma configuração a partir de um arquivo comprimido.
 * 
 * @param input_file Caminho do arquivo .tar.gz.
 * @return Result 
 */
Result restore_gnome_backup(const char *input_file);

#endif // BACKUP_H
