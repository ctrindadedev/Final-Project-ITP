#ifndef BARCODE_UTILS_H
#define BARCODE_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int validar_identificador(const char *identificador);
int gerar_codigo_pbm(const char *identificador, int espaco_lateral, int pixels_por_area, int altura, const char *arquivo_saida);

#endif
