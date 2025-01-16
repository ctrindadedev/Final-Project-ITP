#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define DEFAULT_SPACING 4
#define DEFAULT_BAR_WIDTH 3
#define DEFAULT_HEIGHT 50

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Formato esperado: %s <Identificador EAN-8> <Arquivo PBM de Saida> [Espacamento (Opcional)] [Largura (Opcional)] [Altura (Opcional)]\n", argv[0]);
        return EXIT_FAILURE;
    }

    BarcodeConfig config = {
        .identifier = argv[1],
        .filename = argv[2],
        .spacing = (argc > 3) ? atoi(argv[3]) : DEFAULT_SPACING,
        .bar_width = (argc > 4) ? atoi(argv[4]) : DEFAULT_BAR_WIDTH,
        .height = (argc > 5) ? atoi(argv[5]) : DEFAULT_HEIGHT
    };

    // Confirma se o arquivo tem a extensao ".pbm"
    const char *extension = strrchr(config.filename, '.');
    if (!extension || strcmp(extension, ".pbm") != 0) {
        fprintf(stderr, "Erro: O arquivo de saida deve terminar com a extensao .pbm\n");
        return EXIT_FAILURE;
    }

    if (config.spacing <= 0 || config.bar_width <= 0 || config.height <= 0) {
        fprintf(stderr, "Erro: Os valores de espaco, largura e altura precisam ser maiores que zero.\n");
        return EXIT_FAILURE;
    }

    // Verifica se o arquivo ja existe
    FILE *file = fopen(config.filename, "r");
    if (file) {
        fclose(file);
        printf("O arquivo '%s' ja existe. Deseja substitui-lo? (S/N): ", config.filename);

        char resposta;
        if (scanf(" %c", &resposta) != 1) {
            fprintf(stderr, "Erro: Nao foi possivel capturar a resposta do usuario.\n");
            return EXIT_FAILURE;
        }

        if (resposta != 'S' && resposta != 's') {
            fprintf(stderr, "Erro: Arquivo existente nao foi substituido.\n");
            return EXIT_FAILURE;
        }
    }

    if (!gerar_codigo(&config)) {
        return EXIT_FAILURE;
    }

    printf("Codigo de barras gerado com sucesso no arquivo: %s\n", config.filename);
    return EXIT_SUCCESS;
}
