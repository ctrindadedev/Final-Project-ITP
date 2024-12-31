//                            Recebendo entradas Númericas                    //



//Código binário que será passado para o desenho (Exemplo com o código da página 5 do PDF do projeto)
    int codigo[] = {1,0,1,0,1,0,0,0,1,1,0,0,0,1,1,0,1,0,0,1,1,0,0,1,0,1,1,1,0,1,1,0,1,0,1,0,1,1,1,0,0,1,0,1,0,0,0,1,0,0,11,0,1,1,0,0,1,0,0,1,1,1,0,1,0,1};




//                            Desenhando no PBM                     //

//Definindo como exemplo o número máximo de colunas e linhas do nosso desenho (x e y, respectivamente) -> Precisa ser alterado , pois o que vai definir a área de desenho é a entrada do usuário
#define MAX_X 800
#define MAX_Y 500

//Cores 
#define BRANCO 0
#define PRETO 1


//Matriz que será preenchida com os 0's e 1's
int img[MAX_Y][MAX_X];

//Função para garantir que o arquivo todo esteja branco antes de um novo desenho
void limpa()
{
    for (int linha = 0; linha < MAX_Y; linha++)
    {
        for (int col = 0; col < MAX_X; col++)
        {
            img[linha][col] = BRANCO; //Acessando cada linha coluna e pondo o valor BRANCO
        }
    }
}


//Função para receber os parametros iniciais, no nosso projeto todos desenhos vão receber o valor PRETO na cor

void set(int espacamento, int largura, int altura, int cor) 
{
    for (int linha = 0; linha < altura; linha++) //Limitando para percorrer a matriz até a area vertical definida pelas linhas
    {
        //Está sobrando espaço do lado esquerdo, mas isso se deve a alteração que ainda vamos fazer quanto o máximo de espaço da matriz.
        //Também falta criar uma condição para ser nosso espaço na parte de cima 

        for (int col = espacamento; col < espacamento + largura; col++)  //Inicia onde será definido o espaço lateral e percorre até o fim do espaçamento + a area horizontal do desenho
        {
        //CondicionaL para checar se está dentro dos limites da matriz imagem 
            if (col < MAX_X && linha < MAX_Y) 
            { 
                img[linha][col] = cor;        
            }
        }
    }
}

//Desenhar o código de Barras + Junção da função set com uma implementação de espaços
void desenhar(int largura_barra, int altura_barra, int espacamento_lateral) 
{
    int espacamento = espacamento_lateral;  // Começa após o espaçamento

//Espaçamento entre Barras
    for (int i = 0; i < sizeof(codigo) / sizeof(codigo[0]); i++) 
    {
        if (codigo[i] == 1) 
        {  
            set(espacamento, largura_barra, altura_barra, PRETO);
        }
        espacamento += largura_barra;  // Avança para a próxima posição
    }
}