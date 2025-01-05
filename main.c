#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "barcode_utils.h"

int main(int argc, char *argv[]) {
    // Verifica se o identificador foi passado como argumento
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <identificador> [espaçamento] [pixels_por_area] [altura] [arquivo_saida]\n", argv[0]);
        return 1;
    }

    char *identificador = argv[1];
    int espaco_lateral = (argc > 2) ? atoi(argv[2]) : 4;
    int pixels_por_area = (argc > 3) ? atoi(argv[3]) : 3;
    int altura = (argc > 4) ? atoi(argv[4]) : 50;
    char *arquivo_saida = (argc > 5) ? argv[5] : "codigo_barras.pbm";

    // Valida o identificador
    if (!validar_identificador(identificador)) {
        fprintf(stderr, "Identificador inválido. Deve conter exatamente 8 dígitos numéricos e um dígito verificador válido.\n");
        return 1;
    }

    // Gera o código de barras
    if (gerar_codigo_pbm(identificador, espaco_lateral, pixels_por_area, altura, arquivo_saida)) {
        printf("Imagem do código de barras gerada com sucesso: %s\n", arquivo_saida);
    } else {
        fprintf(stderr, "Erro ao gerar a imagem do código de barras.\n");
    }

    return 0;
}
