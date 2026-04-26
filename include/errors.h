#ifndef ERRORS_H
#define ERRORS_H

#include <stdint.h>
#include <stdalign.h>

/**
 * @brief Sistema de erros baseado em bitmask.
 * Cada bit representa uma categoria de erro específica.
 */
typedef enum {
    SUCCESS            = 0,
    ERR_GSETTINGS      = (1u << 0),
    ERR_FILESYSTEM     = (1u << 1),
    ERR_COMPRESSION    = (1u << 2),
    ERR_PERMISSION     = (1u << 3),
    ERR_MEMORY         = (1u << 4),
    ERR_THEME_NOT_FOUND = (1u << 5),
    ERR_UNKNOWN        = (1u << 31)
} ErrorMask;

/**
 * @brief Estrutura de resultado alinhada para evitar padding ineficiente.
 * Organizada do maior membro para o menor.
 */
typedef struct {
    alignas(16) uint64_t timestamp;  // Alinha a struct pelo primeiro membro
    ErrorMask mask;                  // 4 bytes
    uint32_t reserved;               // 4 bytes - Padding explícito
} Result;

#endif // ERRORS_H
