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

#endif // BACKUP_H
