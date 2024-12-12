#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_NOME 50

// Estrutura para armazenar os dados de uma transação
typedef struct {
    char operacao;         // 'I' = Inclusão, 'A' = Alteração, 'E' = Exclusão
    int codigo;            // Código da mercadoria
    char nome[TAM_NOME];   // Nome da mercadoria
    int quantidade;        // Quantidade da mercadoria
} Transacao;

// Função para incluir uma transação no arquivo
void incluir_transacao(const char *arquivo_transacoes) {
    FILE *f_transacoes = fopen(arquivo_transacoes, "ab");  // Abre o arquivo para anexar (append)

    if (f_transacoes == NULL) {
        printf("Erro ao abrir o arquivo de transações.\n");
        return;
    }

    Transacao transacao;

    // Solicitar os dados da transação
    printf("Digite a operação (I - Inclusão, A - Alteração, E - Exclusão): ");
    scanf(" %c", &transacao.operacao);  // O espaço antes do %c serve para consumir o caractere de nova linha do buffer

    printf("Digite o código da mercadoria: ");
    scanf("%d", &transacao.codigo);
    getchar();  // Limpar o buffer do teclado

    printf("Digite o nome da mercadoria: ");
    fgets(transacao.nome, TAM_NOME, stdin);
    transacao.nome[strcspn(transacao.nome, "\n")] = 0;  // Remover o '\n' do final da string

    printf("Digite a quantidade: ");
    scanf("%d", &transacao.quantidade);

    // Escrever a transação no arquivo binário
    fwrite(&transacao, sizeof(Transacao), 1, f_transacoes);
    fclose(f_transacoes);

    printf("Transação registrada com sucesso!\n");
}

// Função para ler as transações do arquivo e exibir
void ler_transacoes(const char *arquivo_transacoes) {
    FILE *f_transacoes = fopen(arquivo_transacoes, "rb");  // Abre o arquivo para leitura

    if (f_transacoes == NULL) {
        printf("Erro ao abrir o arquivo de transações.\n");
        return;
    }

    Transacao transacao;

    printf("\nTransações registradas:\n");
    printf("Operação\tCódigo\tNome\tQuantidade\n");

    // Lê e exibe todas as transações
    while (fread(&transacao, sizeof(Transacao), 1, f_transacoes)) {
        printf("%c\t %d \t %s \t %d \n", transacao.operacao, transacao.codigo, transacao.nome, transacao.quantidade);
    }

    fclose(f_transacoes);
}

// Função principal
int main() {
    const char *arquivo_transacoes = "transacoes.bin";

    int opcao;
    do {
        printf("\nMenu de Transações:\n");
        printf("1. Incluir Transação\n");
        printf("2. Ler Transações\n");
        printf("3. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                incluir_transacao(arquivo_transacoes);
                break;
            case 2:
                ler_transacoes(arquivo_transacoes);
                break;
            case 3:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while (opcao != 3);

    return 0;
}
