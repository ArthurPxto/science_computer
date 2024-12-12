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

// Estrutura para armazenar os dados de uma transação
typedef struct {
    char operacao;         // 'I' = Inclusão, 'A' = Alteração, 'E' = Exclusão
    int codigo;            // Código da mercadoria
    char nome[TAM_NOME];   // Nome da mercadoria
    int quantidade;        // Quantidade da mercadoria
} Transacao;

// Função para atualizar o arquivo mestre
void atualizar_arquivo_mestre(const char *arquivo_mestre, const char *arquivo_transacoes) {
    FILE *f_mestre = fopen(arquivo_mestre, "r+b"); // Abre o arquivo mestre para leitura e escrita
    FILE *f_transacoes = fopen(arquivo_transacoes, "rb"); // Abre o arquivo de transações para leitura
    
    if (f_mestre == NULL || f_transacoes == NULL) {
        printf("Erro ao abrir os arquivos.\n");
        return;
    }

    Mercadoria mercadoria;
    Transacao transacao;
    
    // Processa as transações
    while (fread(&transacao, sizeof(Transacao), 1, f_transacoes)) {
        char operacao = transacao.operacao;
        
        if (operacao == 'I') { // Inclusão
            fseek(f_mestre, 0, SEEK_SET);
            int encontrado = 0;

            // Verifica se a mercadoria já existe (código único)
            while (fread(&mercadoria, sizeof(Mercadoria), 1, f_mestre)) {
                if (mercadoria.codigo == transacao.codigo) {
                    encontrado = 1;
                    break;
                }
            }

            if (!encontrado) {
                // Se não encontrar, adiciona a mercadoria
                fseek(f_mestre, 0, SEEK_END);
                mercadoria.codigo = transacao.codigo;
                strncpy(mercadoria.nome, transacao.nome, TAM_NOME);
                mercadoria.quantidade = transacao.quantidade;
                fwrite(&mercadoria, sizeof(Mercadoria), 1, f_mestre);
            } else {
                printf("Erro: Mercadoria com código %d já existe.\n", transacao.codigo);
            }

        } else if (operacao == 'A') { // Alteração
            fseek(f_mestre, 0, SEEK_SET);
            int encontrado = 0;

            // Busca a mercadoria e altera
            while (fread(&mercadoria, sizeof(Mercadoria), 1, f_mestre)) {
                if (mercadoria.codigo == transacao.codigo) {
                    encontrado = 1;
                    mercadoria.quantidade = transacao.quantidade;
                    strncpy(mercadoria.nome, transacao.nome, TAM_NOME);
                    fseek(f_mestre, -sizeof(Mercadoria), SEEK_CUR);
                    fwrite(&mercadoria, sizeof(Mercadoria), 1, f_mestre);
                    break;
                }
            }

            if (!encontrado) {
                printf("Erro: Mercadoria com código %d não encontrada para alteração.\n", transacao.codigo);
            }

        } else if (operacao == 'E') { // Exclusão
            FILE *f_temp = fopen("temp.bin", "wb"); // Arquivo temporário para exclusão

            if (f_temp == NULL) {
                printf("Erro ao criar arquivo temporário.\n");
                fclose(f_mestre);
                fclose(f_transacoes);
                return;
            }

            fseek(f_mestre, 0, SEEK_SET);
            int encontrado = 0;

            // Copia todas as mercadorias para o arquivo temporário, exceto a excluída
            while (fread(&mercadoria, sizeof(Mercadoria), 1, f_mestre)) {
                if (mercadoria.codigo != transacao.codigo) {
                    fwrite(&mercadoria, sizeof(Mercadoria), 1, f_temp);
                } else {
                    encontrado = 1;
                }
            }

            if (!encontrado) {
                printf("Erro: Mercadoria com código %d não encontrada para exclusão.\n", transacao.codigo);
            }

            fclose(f_mestre);
            fclose(f_temp);

            // Substitui o arquivo mestre pelo arquivo temporário
            remove(arquivo_mestre);
            rename("temp.bin", arquivo_mestre);

            // Reabre o arquivo mestre para as próximas operações
            f_mestre = fopen(arquivo_mestre, "r+b");
        }
    }

    fclose(f_mestre);
    fclose(f_transacoes);
}

int main() {

    atualizar_arquivo_mestre("arquivo_mestre.bin", "transacoes.bin");
        
    return 0;
}