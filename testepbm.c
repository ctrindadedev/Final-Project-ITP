#include <stdio.h>
#include <stdlib.h>
#include "functions_pbm.h"

//Função para implementarmos a matriz no arquivo PBM (Ou seja, salvar tudo que a gente fez dentro da matriz img)
void print()
{
    FILE *fp;
    fp = fopen("barra.pbm", "w+"); //Forma de acessar o pbm, w+ significa modo de escrever 
    fprintf(fp, "P1\n"); //P1 é o que precisa estar antes de qualquer matriz, para indicar que o arquivo será em preto e branco
    fprintf(fp, "%d %d\n", MAX_X, MAX_Y);
    for (int linha = 0; linha < MAX_Y; linha++) //Percorrendo de Baixo para cima, ultima linha em baixo
    {
        for (int col = 0; col < MAX_X; col++)
        {
            fprintf(fp, "%d ", img[linha][col]); // Imprimindo em todas linhas e colunas do arquivo
        }
     
        fprintf(fp, "\n");
      
    }
    fclose(fp);
    }

    int main()
    {
        limpa();
        // Configurações do código de barras
        int largura_barras = 3; // largura do código em pixels 
        int altura_barras = 50; //Altura também é pixels
        int espacamento_lateral = 4;

        desenhar(largura_barras, altura_barras, espacamento_lateral);
        print(); 
        return 0;
    }