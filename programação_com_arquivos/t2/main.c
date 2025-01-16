#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TAMANHO_JANELA 20 // Tamanho do buffer deslizante
#define TAMANHO_BUFFER_A_VISTA 10 // Tamanho do buffer de visualização à frente

// Estrutura para armazenar os tokens (posição, comprimento, próximo caractere)
typedef struct {
    int deslocamento;
    int comprimento;
    char proximoCaractere;
} Token;

// Função de compressão utilizando LZ77
void comprimir(const char *entrada, Token *tokens, int *quantidadeTokens) {
    int tamanhoEntrada = strlen(entrada);
    int cursor = 0; // Cursor para percorrer a entrada
    *quantidadeTokens = 0; // Inicializa a contagem de tokens

    while (cursor < tamanhoEntrada) {
        int comprimentoMaximo = 0;
        int melhorDeslocamento = 0;

        // Percorre o buffer deslizante para encontrar a melhor correspondência
        for (int i = 1; i <= TAMANHO_JANELA && cursor - i >= 0; i++) {
            int comprimentoAtual = 0;
            // Tenta encontrar a correspondência mais longa
            while (comprimentoAtual < TAMANHO_BUFFER_A_VISTA &&
                   cursor + comprimentoAtual < tamanhoEntrada &&
                   entrada[cursor - i + comprimentoAtual] == entrada[cursor + comprimentoAtual]) {
                comprimentoAtual++;
            }

            // Atualiza a melhor correspondência encontrada
            if (comprimentoAtual > comprimentoMaximo) {
                comprimentoMaximo = comprimentoAtual;
                melhorDeslocamento = i;
            }
        }

        // Preenche o token com as informações encontradas
        Token token;
        token.deslocamento = melhorDeslocamento;
        token.comprimento = comprimentoMaximo;
        token.proximoCaractere = (cursor + comprimentoMaximo < tamanhoEntrada) ? entrada[cursor + comprimentoMaximo] : '\0';
        tokens[(*quantidadeTokens)++] = token;

        // Avança o cursor
        cursor += comprimentoMaximo + 1;
    }
}

// Função de descompressão utilizando os tokens do LZ77
void descomprimir(const Token *tokens, int quantidadeTokens, char *saida) {
    int cursor = 0; // Cursor para preencher a saída

    for (int i = 0; i < quantidadeTokens; i++) {
        Token token = tokens[i];
        
       // Copia os caracteres correspondentes a partir da janela deslizante
        for (int j = 0; j < token.comprimento; j++) {
            if (cursor - token.deslocamento + j >= 0) { // Verifica se o índice é válido
                saida[cursor] = saida[cursor - token.deslocamento + j];
                cursor++;
            }
        }


        // Adiciona o próximo caractere ao buffer de saída
        if (token.proximoCaractere != '\0') {
            saida[cursor++] = token.proximoCaractere;
        }
    }

    // Finaliza a string de saída
    saida[cursor] = '\0';
}

int main() {
    const char *entrada = "00110110001111000";
    Token tokens[100];
    int quantidadeTokens;

    printf("Texto original: %s\n", entrada);

    // Compressão
    comprimir(entrada, tokens, &quantidadeTokens);

    printf("\nTokens:\n");
    for (int i = 0; i < quantidadeTokens; i++) {
        printf("(Deslocamento: %d, Comprimento: %d, Próximo caractere: '%c')\n", tokens[i].deslocamento, tokens[i].comprimento, tokens[i].proximoCaractere);
    }

    // Descompressão
    char saida[100];
    descomprimir(tokens, quantidadeTokens, saida);

    printf("\nTexto descomprimido: %s\n", saida);

    return 0;
}
