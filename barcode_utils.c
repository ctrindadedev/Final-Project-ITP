#include "barcode_utils.h"
#include <stdio.h>
#include <string.h>

// valida se o identificador tem 8 dígitos e o dígito verificador está correto
int validar_identificador(const char *identificador) {
    if (strlen(identificador) != 8) return 0;

    // digito verficador - se for par peso 3, impar peso 1 - pag 2
    int soma = 0;
    for (int i = 0; i < 7; i++) {
        int digito = identificador[i] - '0';
        soma += (i % 2 == 0) ? digito * 3 : digito * 1;
    }

    int digito_verificador = (10 - (soma % 10)) % 10; //qnt falta para o prox multiplo de 10
    return (digito_verificador == identificador[7] - '0'); // compara o começo c o final
}

// d acordo c o l-code e r-code - arquivo PBM com o código de barras - pag 3 e 4
int gerar_codigo_pbm(const char *identificador, int espaco_lateral, int pixels_por_area, int altura_barras, const char *arquivo_saida) {
    const char *L_CODE[] = {"0001101", "0011001", "0010011", "0111101", "0100011", "0110001", "0101111", "0111011", "0110111", "0001011"};
    const char *R_CODE[] = {"1110010", "1100110", "1101100", "1000010", "1011100", "1001110", "1010000", "1000100", "1001000", "1110100"};

    if (strlen(identificador) != 8) {
        printf("Erro: O identificador deve ter exatamente 8 dígitos.\n");
        return 0;
    }

    // largura total do código de barras - pag 3
    int largura = espaco_lateral * 2 + pixels_por_area * 95; // 95 = 3 (início) + 28 (L-code) + 5 (central) + 28 (R-code) + 3 (final)

    // Abre o arquivo para escrita
    FILE *arquivo = fopen(arquivo_saida, "w");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo");
        return 0;
    }

    // Escreve o cabeçalho PBM
    fprintf(arquivo, "P1\n%d %d\n", largura, altura_barras);

    // Gera cada linha do código de barras
    for (int y = 0; y < altura_barras; y++) {
        for (int x = 0; x < largura; x++) {
            int is_bar = 0;

            if (x >= espaco_lateral && x < largura - espaco_lateral) {
                int pos = (x - espaco_lateral) / pixels_por_area;

                // Determina a área do código de barras
                if (pos < 3) { // Marcador inicial
                    is_bar = (pos == 0 || pos == 2);
                } else if (pos < 31) { // L-code (primeiros 4 dígitos)
                    int digito = identificador[(pos - 3) / 7] - '0';
                    is_bar = L_CODE[digito][(pos - 3) % 7] - '0';
                } else if (pos < 36) { // Marcador central
                    is_bar = ((pos - 31) % 2 == 1);
                } else if (pos < 64) { // R-code (últimos 4 dígitos)
                    int digito = identificador[4 + (pos - 36) / 7] - '0';
                    is_bar = R_CODE[digito][(pos - 36) % 7] - '0';
                } else { // Marcador final
                    is_bar = (pos - 64) % 2 == 0;
                }
            }

            // Escreve o pixel
            fprintf(arquivo, "%d", is_bar);
        }
        fprintf(arquivo, "\n");
    }

    fclose(arquivo);
    return 1;
}
