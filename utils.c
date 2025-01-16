#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

/*
    Matrizes de caracteres para os dois tipos de codificações que o padrão EAN-8 utiliza
*/
const char *ean8_l_patterns[10] = {
    "0001101", "0011001", "0010011", "0111101", "0100011",
    "0110001", "0101111", "0111011", "0110111", "0001011"
};

const char *ean8_r_patterns[10] = {
    "1110010", "1100110", "1101100", "1000010", "1011100",
    "1001110", "1010000", "1000100", "1001000", "1110100"
};

/*
    Função para validar o identificador EAN-8
*/
bool validar_identificador(const char *identifier) {
    if (strlen(identifier) != 8) {
        fprintf(stderr, "Erro! O identificador precisa ter exatamente 8 dígitos.\n");
        return false;
    }
    
    for (int i = 0; i < 8; i++) {
        if (identifier[i] < '0' || identifier[i] > '9') {
            fprintf(stderr, "Erro! O identificador contêm caracteres não numéricos.\n");
            return false;
        }
    }
    
    int sum = 0;
    for (int i = 0; i < 7; i++) {
        int digit = identifier[i] - '0';
        sum += (i % 2 == 0) ? digit * 3 : digit;
    }
    
    int check_digit = (10 - (sum % 10)) % 10;
    if (check_digit != (identifier[7] - '0')) {
        fprintf(stderr, "Erro! Identificador Inválido.\n");
        return false;
    }
    return true;
}

/*
    Função para gerar o código de barras a partir de um identificador EAN-8
*/
bool gerar_codigo(const BarcodeConfig *config) {
    if (!validar_identificador(config->identifier)) {
        return false;
    }

    FILE *file = fopen(config->filename, "w");
    if (!file) {
        fprintf(stderr, "Erro! Não foi possível criar o arquivo %s.\n", config->filename);
        return false;
    }

    // Marcadores de início, centro e fim
    const char *start_marker = "101";
    const char *center_marker = "01010";
    const char *end_marker = "101";

    int code_width = config->spacing * 2 + (3 + 28 + 5 + 28 + 3) * config->bar_width;
    int total_height = config->height + 2 * config->spacing;

    fprintf(file, "P1\n%d %d\n", code_width, total_height);

    // Margem superior
    for (int y = 0; y < config->spacing; y++) {
        for (int x = 0; x < code_width; x++) {
            fprintf(file, "0 ");
        }
        fprintf(file, "\n");
    }

    // Corpo do código de barras
    for (int y = 0; y < config->height; y++) {
        for (int x = 0; x < config->spacing; x++) {
            fprintf(file, "0 ");
        }

        // Início do código de barras (start_marker)
        for (int i = 0; i < 3; i++) {
            for (int w = 0; w < config->bar_width; w++) {
                fprintf(file, "%d ", start_marker[i] - '0');
            }
        }

        // Padrões para os 4 primeiros dígitos (L-patterns)
        for (int i = 0; i < 4; i++) {
            const char *pattern = ean8_l_patterns[config->identifier[i] - '0'];
            for (int j = 0; j < 7; j++) {
                for (int w = 0; w < config->bar_width; w++) {
                    fprintf(file, "%d ", pattern[j] - '0');
                }
            }
        }

        // Centro do código de barras (center_marker)
        for (int i = 0; i < 5; i++) {
            for (int w = 0; w < config->bar_width; w++) {
                fprintf(file, "%d ", center_marker[i] - '0');
            }
        }

        // Padrões para os 4 últimos dígitos (R-patterns)
        for (int i = 4; i < 8; i++) {
            const char *pattern = ean8_r_patterns[config->identifier[i] - '0'];
            for (int j = 0; j < 7; j++) {
                for (int w = 0; w < config->bar_width; w++) {
                    fprintf(file, "%d ", pattern[j] - '0');
                }
            }
        }

        // Fim do código de barras (end_marker)
        for (int i = 0; i < 3; i++) {
            for (int w = 0; w < config->bar_width; w++) {
                fprintf(file, "%d ", end_marker[i] - '0');
            }
        }

        for (int x = 0; x < config->spacing; x++) {
            fprintf(file, "0 ");
        }

        fprintf(file, "\n");
    }

    // Margem inferior
    for (int y = 0; y < config->spacing; y++) {
        for (int x = 0; x < code_width; x++) {
            fprintf(file, "0 ");
        }
        fprintf(file, "\n");
    }

    fclose(file);
    return true;
}

/*
    Função para verificar o cabeçalho do arquivo PBM
*/
bool verificar_cabecalho(FILE *file, int *bar_width, int *height) {
    char line[256];

    // Pular linhas ou espaços em branco no início do arquivo
    int ch;
    do {
        ch = fgetc(file);
    } while (isspace(ch));

    if (ch == EOF) {
        fprintf(stderr, "Erro! O arquivo está vazio ou mal formatado.\n");
        return false;
    }

    // Recolocar o caractere "não espaço" de volta ao fluxo
    ungetc(ch, file);

    // Ler o formato (primeira linha do arquivo)
    if (!fgets(line, sizeof(line), file)) {
        fprintf(stderr, "Erro! Não foi possível ler o formato.\n");
        return false;
    }

    // Remover caracteres invisíveis
    line[strcspn(line, "\r\n")] = '\0';

    // Verificar o formato P1
    if (strcmp(line, "P1") != 0) {
        fprintf(stderr, "Erro! O arquivo não segue o formato PBM esperado (P1).\n");
        return false;
    }

    // Ler as dimensões (segunda linha do arquivo)
    if (!fgets(line, sizeof(line), file) || sscanf(line, "%d %d", bar_width, height) != 2) {
        fprintf(stderr, "Erro! Dimensões Inválidas no cabeçalho do arquivo.\n");
        return false;
    }

    // Cabeçalho válido
    printf("%d %d", *bar_width, *height);
    return true;
}

/*
    Função para verificar os marcadores de início, central e de fim do código de barras
*/
bool verificar_marcadores(int **matriz, int bar_width, int height, int N, char *binary_seq) {
    int spacing = (bar_width - (N * 67)) / 2; // Espaçamento lateral
    int pos = 0; // Posição atual no código de barras

    // Extrair a sequência binária
    for (int i = spacing; i < bar_width - spacing; i += N) {
        int sum = 0;
        for (int j = i; j < i + N; j++) {
            sum += matriz[height / 2][j]; // Considerar linha central
        }
        binary_seq[pos++] = (sum > (N / 2)) ? '1' : '0';
    }
    binary_seq[pos] = '\0';

    // Calcular índices dos marcadores
    int start_marker = 0;                    
    int center_marker = 3 + 28;               // Após marcador inicial + L-code
    int end_marker = center_marker + 5 + 28;         // Após marcador central + R-code

    // Verificar marcadores
    if (strncmp(&binary_seq[start_marker], "101", 3) != 0) {
        printf("Erro! Marcador de Início Inválido.\n");
        return false;
        
    } if (strncmp(&binary_seq[center_marker], "01010", 5) != 0) {
        printf("Erro! Marcador Central Inválido.\n");
        return false;
        
    } if (strncmp(&binary_seq[end_marker], "101", 3) != 0) {
        printf("Erro! Marcador de Fim Inválido.\n");
        return false;
    }

    return true;
}

/*
    Função para extrair o identificador decodificado do código de barras
*/
bool extrair_identificador(const char *binary_seq, char *identifier) {
    int pos = 0; // Posição atual no código de barras

    // Decodificar L-code (4 primeiros dígitos)
    for (int i = 3; i < 31; i += 7) {
        bool found = false;
        for (int j = 0; j < 10; j++) {
            if (strncmp(&binary_seq[i],ean8_l_patterns[j], 7) == 0) {
                identifier[pos++] = '0' + j;
                found = true;
                break;
            }
        }
        if (!found) {
            printf("Erro! Sequência L-Code Inválida.\n");
            return false;
        }
    }

    // Decodificar R-code (4 últimos dígitos)
    for (int i = 36; i < 64; i += 7) {
        bool found = false;
        for (int j = 0; j < 10; j++) {
            if (strncmp(&binary_seq[i], ean8_r_patterns[j], 7) == 0) {
                identifier[pos++] = '0' + j;
                found = true;
                break;
            }
        }
        if (!found) {
            printf("Erro! Sequência R-Code Inválida.\n");
            return false;
        }
    }

    identifier[pos] = '\0';
    return true;
}