#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_NOME 50

// Estrutura para armazenar os dados de mercadoria
typedef struct {
    int codigo;
    char nome[TAM_NOME];
    int quantidade;
} Mercadoria;

// Estrutura para armazenar a transacao
typedef struct {
    char operacao;
    int codigo;
    char nome[TAM_NOME];
    int quantidade;
} Transacao;

// Função para ler o arquivo mestre e escrever em arquivo de texto
void ler_arquivo_mestre_para_txt(const char *arquivo_mestre, const char *arquivo_saida) {
    FILE *f_mestre = fopen(arquivo_mestre, "rb"); // Abre o arquivo mestre para leitura
    FILE *f_saida = fopen(arquivo_saida, "w");    // Abre o arquivo de saída para escrita

    if (f_mestre == NULL || f_saida == NULL) {
        printf("Erro ao abrir os arquivos.\n");
        return;
    }

    Mercadoria mercadoria;

    // Escreve o cabeçalho do arquivo de saída
    fprintf(f_saida, "Código\tNome\tQuantidade\n");

    // Lê os registros do arquivo mestre e escreve no arquivo de saída
    while (fread(&mercadoria, sizeof(Mercadoria), 1, f_mestre)) {
        fprintf(f_saida, "%d\t%s\t%d\n", mercadoria.codigo, mercadoria.nome, mercadoria.quantidade);
    }

    fclose(f_mestre);
    fclose(f_saida);
}

// Função para ler o arquivo de transações e escrever em arquivo de texto
void ler_arquivo_transacoes_para_txt(const char *arquivo_transacoes, const char *arquivo_saida) {
    FILE *f_transacoes = fopen(arquivo_transacoes, "rb"); // Abre o arquivo de transações para leitura
    FILE *f_saida = fopen(arquivo_saida, "w");            // Abre o arquivo de saída para a escrita

    if (f_transacoes == NULL || f_saida == NULL) {
        printf("Erro ao abrir os arquivos.\n");
        return;
    }

    Transacao transacao;

    // Escreve o cabeçalho do arquivo de saída (se necessário)
    fprintf(f_saida, "Operação\tCódigo\tNome\tQuantidade\n");

    // Lê as transações do arquivo e escreve no arquivo de saída
    while (fread(&transacao, sizeof(Transacao), 1, f_transacoes)){


        // Escreve os dados no arquivo de saída
        fprintf(f_saida, "%c \t\t %d \t\t %s \t\t %d \n", transacao.operacao, transacao.codigo, transacao.nome, transacao.quantidade);
    }

    fclose(f_transacoes);
    fclose(f_saida);
}

int main() {
    // Chama a função para ler o arquivo mestre e escrever no arquivo de texto
    ler_arquivo_mestre_para_txt("arquivo_mestre.bin", "arquivo_mestre.txt");

    // Chama a função para ler o arquivo de transações e escrever no arquivo de texto
    ler_arquivo_transacoes_para_txt("transacoes.bin", "transacoes.txt");

    printf("Arquivos convertidos com sucesso!\n");

    return 0;
}
