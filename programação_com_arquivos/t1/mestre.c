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

// Função para escrever o arquivo binário ordenado
void escrever_arquivo_ordenado(FILE *f_mestre, Mercadoria mercadoria) {
    fseek(f_mestre, 0, SEEK_SET); // Vai para o início do arquivo
    Mercadoria temp;
    long pos = ftell(f_mestre);

    // Verificar se o arquivo está vazio
    if (fread(&temp, sizeof(Mercadoria), 1, f_mestre) == 0) {
        fwrite(&mercadoria, sizeof(Mercadoria), 1, f_mestre);
        return;
    }

    // Insere mercadoria no lugar correto para manter a ordem
    while (fread(&temp, sizeof(Mercadoria), 1, f_mestre)) {
        if (mercadoria.codigo < temp.codigo) {
            fseek(f_mestre, pos, SEEK_SET);  // Volta para a posição onde deve ser inserido
            fwrite(&mercadoria, sizeof(Mercadoria), 1, f_mestre);
            return;
        }
        pos = ftell(f_mestre);
    }

    // Caso seja o último registro ou maior que todos, insere no final
    fwrite(&mercadoria, sizeof(Mercadoria), 1, f_mestre);
}

// Função para incluir mercadoria
void incluir_mercadoria(const char *arquivo_mestre) {
    FILE *f_mestre = fopen(arquivo_mestre, "r+b");
    if (f_mestre == NULL) {
        f_mestre = fopen(arquivo_mestre, "wb");  // Cria o arquivo se não existir
    }

    Mercadoria mercadoria;
    printf("Digite o código da mercadoria: ");
    scanf("%d", &mercadoria.codigo);
    getchar();  // Limpar o buffer do teclado

    printf("Digite o nome da mercadoria: ");
    fgets(mercadoria.nome, TAM_NOME, stdin);
    mercadoria.nome[strcspn(mercadoria.nome, "\n")] = 0; // Remove a nova linha

    printf("Digite a quantidade: ");
    scanf("%d", &mercadoria.quantidade);

    escrever_arquivo_ordenado(f_mestre, mercadoria);

    fclose(f_mestre);
    printf("Mercadoria incluída com sucesso!\n");
}

// Função para alterar mercadoria
void alterar_mercadoria(const char *arquivo_mestre) {
    FILE *f_mestre = fopen(arquivo_mestre, "r+b");
    if (f_mestre == NULL) {
        printf("Arquivo mestre não encontrado.\n");
        return;
    }

    int codigo;
    printf("Digite o código da mercadoria a ser alterada: ");
    scanf("%d", &codigo);

    Mercadoria mercadoria;
    int encontrado = 0;

    // Busca a mercadoria no arquivo
    while (fread(&mercadoria, sizeof(Mercadoria), 1, f_mestre)) {
        if (mercadoria.codigo == codigo) {
            encontrado = 1;
            printf("Mercadoria encontrada!\n");

            printf("Digite o novo nome da mercadoria: ");
            getchar();  // Limpar o buffer do teclado
            fgets(mercadoria.nome, TAM_NOME, stdin);
            mercadoria.nome[strcspn(mercadoria.nome, "\n")] = 0; // Remove a nova linha

            printf("Digite a nova quantidade: ");
            scanf("%d", &mercadoria.quantidade);

            fseek(f_mestre, -sizeof(Mercadoria), SEEK_CUR); // Volta para o registro encontrado
            fwrite(&mercadoria, sizeof(Mercadoria), 1, f_mestre);
            printf("Mercadoria alterada com sucesso!\n");
            break;
        }
    }

    if (!encontrado) {
        printf("Mercadoria com código %d não encontrada.\n", codigo);
    }

    fclose(f_mestre);
}

// Função para excluir mercadoria
void excluir_mercadoria(const char *arquivo_mestre) {
    FILE *f_mestre = fopen(arquivo_mestre, "r+b");
    if (f_mestre == NULL) {
        printf("Arquivo mestre não encontrado.\n");
        return;
    }

    int codigo;
    printf("Digite o código da mercadoria a ser excluída: ");
    scanf("%d", &codigo);

    FILE *f_temp = fopen("temp.bin", "wb"); // Arquivo temporário para gravar os dados restantes
    if (f_temp == NULL) {
        printf("Erro ao criar arquivo temporário.\n");
        fclose(f_mestre);
        return;
    }

    Mercadoria mercadoria;
    int encontrado = 0;

    // Copia as mercadorias para o arquivo temporário, excluindo a mercadoria com o código informado
    while (fread(&mercadoria, sizeof(Mercadoria), 1, f_mestre)) {
        if (mercadoria.codigo != codigo) {
            fwrite(&mercadoria, sizeof(Mercadoria), 1, f_temp);
        } else {
            encontrado = 1;
        }
    }

    if (encontrado) {
        printf("Mercadoria excluída com sucesso!\n");
        fclose(f_mestre);
        fclose(f_temp);

        remove(arquivo_mestre);       // Apaga o arquivo mestre original
        rename("temp.bin", arquivo_mestre); // Renomeia o arquivo temporário para o nome do arquivo mestre
    } else {
        printf("Mercadoria com código %d não encontrada.\n", codigo);
    }
}

// Função principal
int main() {
    const char *arquivo_mestre = "arquivo_mestre.bin";

    int opcao;
    do {
        printf("\nMenu:\n");
        printf("1. Incluir Mercadoria\n");
        printf("2. Alterar Mercadoria\n");
        printf("3. Excluir Mercadoria\n");
        printf("4. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                incluir_mercadoria(arquivo_mestre);
                break;
            case 2:
                alterar_mercadoria(arquivo_mestre);
                break;
            case 3:
                excluir_mercadoria(arquivo_mestre);
                break;
            case 4:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while (opcao != 4);

    return 0;
}
