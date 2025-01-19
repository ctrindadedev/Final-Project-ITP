# Gerador de Código de Barras EAN-8

Este projeto consiste em um programa que gera um código de barras no formato EAN-8 e o salva em um arquivo no formato PBM. O código de barras gerado pode ser configurado com diferentes opções de espaçamento, largura das barras e altura. Ele também valida o identificador EAN-8 fornecido, garantindo que o código esteja correto antes de gerar o arquivo.

## Requisitos

- Compilador C (GCC, Clang, etc.)
- Sistema operacional baseado em Unix (Linux, macOS) ou Windows
- Biblioteca padrão C (stdio.h, stdlib.h, etc.)

## Como Usar

### Compilação

1. Clone o repositório ou baixe os arquivos do código.
2. Compile o programa usando o comando:
   
   ```
   gcc -o gerador_codigo_barras gerador_pbm.c
   ```

### Execução

Para rodar o programa, use o seguinte formato:

```
./gerador_codigo_barras <Identificador EAN-8> <Arquivo PBM de Saída> [Espaçamento (Opcional)] [Largura (Opcional)] [Altura (Opcional)]
```

**Parâmetros:**
- `<Identificador EAN-8>`: O identificador de 8 dígitos (EAN-8) que será utilizado para gerar o código de barras.
- `<Arquivo PBM de Saída>`: O nome do arquivo onde o código de barras será salvo. Deve ter a extensão `.pbm`.
- `[Espaçamento]`: O valor do espaçamento entre as barras (opcional, padrão é 4).
- `[Largura]`: A largura de cada barra (opcional, padrão é 3).
- `[Altura]`: A altura do código de barras (opcional, padrão é 50).

Exemplo:

```
./gerador_codigo_barras 12345678 codigo_barras.pbm 5 4 60
```

### Validações

1. **Identificador EAN-8**: O programa verifica se o identificador fornecido possui exatamente 8 dígitos numéricos e se a soma dos 7 primeiros dígitos gera um dígito verificador válido.
2. **Extensão do arquivo**: O arquivo de saída deve ter a extensão `.pbm`, caso contrário, o programa exibirá uma mensagem de erro.
3. **Verificação de Arquivo Existente**: Se o arquivo de saída já existir, o programa perguntará se você deseja sobrescrevê-lo.

### Exemplo de Saída

O arquivo PBM gerado será uma imagem binária (formato P1) que pode ser visualizada com ferramentas que suportam o formato PBM.

### Estrutura de Arquivos

- `gerador_codigo_barras.c`: O arquivo principal do programa que gera o código de barras EAN-8.
- `utils.h`: Arquivo de cabeçalho contendo as funções auxiliares para validação do identificador, geração do código de barras e manipulação de arquivos PBM.
- `utils.c`: Implementação das funções auxiliares, incluindo a lógica de validação do identificador EAN-8 e geração do código de barras.

## Funções Importantes

- **validar_identificador**: Valida se o identificador EAN-8 é válido, verificando a quantidade de dígitos e o cálculo do dígito verificador.
- **gerar_codigo**: Gera o código de barras no formato PBM com base no identificador EAN-8 e nas configurações fornecidas.
- **verificar_cabecalho**: Verifica o cabeçalho de um arquivo PBM para garantir que ele esteja no formato esperado.
- **extrair_identificador**: Extrai o identificador EAN-8 de um código de barras no formato PBM.
