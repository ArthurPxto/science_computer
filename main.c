#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct sproduto {
    int cod;
    float preco;
} produto;

//insere no arquivo final utilizando insertion sort
void insertionSort(FILE *arq, produto p, int tam) {
    produto aux;
    int i = tam - 1;

    fseek(arq, 0, SEEK_END);

    if(tam == 0) {
        fwrite(&p, sizeof(produto), 1, arq);
    } else {
        fseek(arq, 0, SEEK_SET);
        fread(&aux, sizeof(produto), 1, arq);

        while(i >= 0 && aux.cod > p.cod) {
            fseek(arq, (i + 1) * sizeof(produto), SEEK_SET);
            fwrite(&aux, sizeof(produto), 1, arq);
            fseek(arq, i * sizeof(produto), SEEK_SET);
            fread(&aux, sizeof(produto), 1, arq);
            i--;
        }

        fseek(arq, (i + 1) * sizeof(produto), SEEK_SET);
        fwrite(&p, sizeof(produto), 1, arq);
    }
}

// recebe um vetor de produtos e cria partições
void criaParticoes(produto *vetor, int n_particoes) {
    FILE* arq;
    char nome_arq[20];

    // cria um arquivo para cada partição
    sprintf(nome_arq, "particao%d.bin", n_particoes);
    arq = fopen(nome_arq, "w+b");

    // verifica se o arquivo não é nulo
    if(arq != NULL) {
        // escreve cada produto no arquivo
        for(int i = 0; i < 5; i++) {
            fwrite(&vetor[i], sizeof(produto), 1, arq);
        }
    }

    // fecha o arquivo
    fclose(arq);
}

void particionarArquivo( FILE* arq, int tam) {
    // cria um vetor de produtos de 5 posições
    produto *vetor = (produto*) malloc( 5 * sizeof( produto ) );
    // variável para contar o número de partições
    int n_particoes = 0;

    //verifica se o arquivo não é nulo
    if ( arq != NULL ) {
        //vai para o início do arquivo
        rewind( arq );
        // le de 5 em 5 produtos e chama a função para ordenar e criar as partições
        while ( fread( vetor, sizeof( produto ), 5, arq ) == 5 ) {
            criaParticoes( vetor, n_particoes );
            n_particoes++;
        }
    }
}
//percorre as partiçoes e insere no arquivo final utilizando inserction sort
void mergeParticoes(int n_particoes) {
    char nome_arq[20];
    FILE* arq_final = fopen("arquivo_final.bin", "a+b");

    produto *vetor = (produto*) malloc( n_particoes * sizeof( produto ) );
    produto p;

    int tam = 0;

    // percorre os arquivos de partição e insere no arquivo final
    for(int i = 0; i < n_particoes; i++) {
        // muda o nome do arquivo para a partição atual
        sprintf(nome_arq, "particao%d.bin", i);
        // abre o arquivo para leitura
        FILE* arq = fopen(nome_arq, "rb");

        //verifica se o arquivo não é nulo
        if(arq != NULL) {
            // le o produto e insere no aqruivo final
            for(int j = 0; j < 5 /* Tamanho universal das partições */; j++) {
                fread(&p, sizeof(produto), 1, arq);
                insertionSort(arq_final, p , tam);
                tam++;
            }
        }
    }
    //fecha o arquivo final
    fclose(arq_final);
}

void ordenaArquivo(FILE* arq) {
    //variável para contar o número de partições
    int n_particoes = 0;

    //verifica se o arquivo não é nulo
    if ( arq != NULL ) {
        //vai para o início do arquivo
        rewind( arq );
        // chama a função para particionar o arquivo
        particionarArquivo(arq, n_particoes);
        // chama a função para juntar as partições
        mergeParticoes(n_particoes);
    }
}

int main(){
    FILE* arq = fopen("arquivo.bin", "r+b");
    
    ordenaArquivo(arq);

    fclose(arq);
    return 0;
}