#include <stdio.h>
#include <glib.h>
#include <gio/gio.h>
#include "errors.h"

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    printf("Gnome Themes Manager inicializado.\n");

    Result res = {
        .timestamp = (uint64_t)g_get_real_time(),
        .mask = SUCCESS,
        .reserved = 0
    };

    if (res.mask == SUCCESS) {
        printf("Sistema de erros validado (Tamanho da struct: %zu bytes).\n", sizeof(Result));
    }

    return 0;
}
