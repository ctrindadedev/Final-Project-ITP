#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdbool.h>

/*
    Struct para encapsular os parâmetros de geração do código de barras
*/
typedef struct {
    const char *identifier; // Identificador EAN-8
    const char *filename;   // Nome do arquivo de saída
    int spacing;            // Espaçamento lateral
    int bar_width;          // Largura das barras
    int height;             // Altura das barras
} BarcodeConfig;

/*
    Protótipos das funções
*/
bool validar_identificador(const char *identifier);

bool gerar_codigo(const BarcodeConfig *config);

bool verificar_cabecalho(FILE *file, int *bar_width, int *height);

bool verificar_marcadores(int **matriz, int bar_width, int height, int N, char *binary_seq);

bool extrair_identificador(const char *binary_seq, char *identifier);


#endif