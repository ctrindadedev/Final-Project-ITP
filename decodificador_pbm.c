#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Erro: Voce precisa informar o nome do arquivo.\n");
        return 1;
    }

    const char *file_name = argv[1];
    FILE *file = fopen(file_name, "r");
    if (file == NULL) {
        fprintf(stderr, "Erro: Arquivo nao pode ser aberto: %s\n", file_name);
        return 1;
    }

    int width, height;

    /* 
        Validar o cabecalho do PBM
    */
    if (!verificar_cabecalho(file, &width, &height)) {
        fclose(file);
        return 1;
    }

    /*
        Alocar memoria para a matriz
    */
    int **matrix = (int **)malloc(height * sizeof(int *));
    for (int i = 0; i < height; i++) {
        matrix[i] = (int *)malloc(width * sizeof(int));
    }

    /*
        Ler os dados da matriz do arquivo PBM
    */
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (fscanf(file, "%d", &matrix[i][j]) != 1) {
                fprintf(stderr, "Erro: Falha ao ler os dados do arquivo PBM.\n");
                fclose(file);
                for (int k = 0; k <= i; k++) free(matrix[k]);
                free(matrix);
                return 1;
            }
        }
    }
    fclose(file);

    /*
        Determinar a largura de cada modulo
    */
    int module_width = width / 67; // 67 areas no padrao EAN-8

    char binary_sequence[68]; // 67 areas + 1 para '\0'
    if (verificar_marcadores(matrix, width, height, module_width, binary_sequence)) {
        printf("\nSequencia binaria validada: %s\n", binary_sequence);

        /*
            Decodificar os digitos
        */
        char identifier[9];
        if (extrair_identificador(binary_sequence, identifier)) {
            printf("Identificador decodificado: %s\n", identifier);
        } else {
            printf("Erro: Falha ao decodificar os digitos.\n");
        }
    } else {
        printf("Erro: Codigo de barras invalido.\n");
    }

    /*
        Liberar memoria alocada
    */
    for (int i = 0; i < height; i++) free(matrix[i]);
    free(matrix);

    return 0;
}
