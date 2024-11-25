#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

typedef struct sproduto {
    int cod;
    float preco;
} produto;

void insere( FILE* arq, int cod, float preco ) {
    produto p;
    p.cod = cod;
    p.preco = preco;
    
    fseek( arq, 0, SEEK_END );    
    fwrite( &p, sizeof( produto ), 1, arq );
}

void imprime_todos( FILE* arq ) {
    produto p;
    int tam;
    int cont = 1;
    
    rewind( arq ); // o mesmo que: fseek( arq, 0, SEEK_SET ); 
    
    tam = fread( &p, sizeof( produto ), 1, arq );
    while ( tam > 0 ) {        
        printf( "\nProduto(%d)..", cont );
        printf( "\n  Codigo...: %d", p.cod );
        printf( "\n  Preco..: %.2f", p.preco );
        printf( "\n" );
        cont++;
        
        tam = fread( &p, sizeof( produto ), 1, arq );
    }
}

void recria_arquivo( FILE* arq, char nome_arq[] ) {
    fclose( arq );    
    remove( nome_arq );
    
    arq = fopen( nome_arq, "a+b" );
}

/* parte alterada pela main  */

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
    char nome_arq[30];

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
    int n_particoes = 0, pos = 0;

    //verifica se o arquivo não é nulo
    if ( arq != NULL ) {
        //vai para o início do arquivo
        rewind( arq );
        // le de 5 em 5 produtos e chama a função para ordenar e criar as partições
        for (int i = 0; i < 5; i++) {
            fread(&vetor[i], sizeof(produto), pos, arq);
            pos++;
        }
    }
}
//percorre as partiçoes e insere no arquivo final utilizando inserction sort
void mergeParticoes(int n_particoes) {
    char nome_arq[30];
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
/* end parte alterada */

void menu() {
    //system( "clear" );
    printf( "******** Agenda ********" );
    printf( "\n(1) Inserir" );
    printf( "\n(2) Mostrar todos" );
    printf( "\n(3) Recriar arquivo" );
    printf( "\n(4) Ordenar arquivo" );
    printf( "\n(0/ESQ) Sair" );
    printf( "\n***********************" );
    printf( "\n\n" );
}

int main() {
    char nome_arq[] = "arq.bin";
    FILE* arq;
    
    int tam;
    char ch;
    int cod;
    float preco;
    
    setlocale(LC_ALL,"Portuguese");
    
    arq = fopen( nome_arq, "a+b" );
    if ( arq == NULL ) {
        printf( "Erro na abertura do arquivo %s.", nome_arq );
        return 1;
    }
    
    ch = '\0';
    do {        
        fflush( stdin );

        menu();
        ch = getchar();
        switch ( ch ) {
            case '1':
                printf( "Informe o codigo: " );
                scanf( "%d", &cod );
                printf( "Informe o preco: " );
                scanf( "%f", &preco );
                
                insere( arq, cod, preco );
                printf( "\nProduto inserido com sucesso!" );
                getchar();                
                break;    
             case '2':
                printf( "***** Lista de produtos *****" );
                printf( "\n\n" );
                imprime_todos( arq );
                printf( "\n\n" );
                printf( "********* lista de produtos ordenada ***********" );
                FILE *arq2 = fopen("arquivo_final.bin", "rb");
                imprime_todos(arq2);
                fclose(arq2);
                printf( "\n\n" );
                getchar();
                break;
            case '3':
                recria_arquivo( arq, nome_arq );
                printf( "Arquivo recriado com sucesso...!" );
                getchar();
                break;
            case '4':
                ordenaArquivo(arq);
                printf( "Arquivo ordenado com sucesso...!" );
                getchar();
                break;
        }               
    } while ( ch != '0' && (int)ch != 27 );
           
    fclose( arq );
   
    return 0;
}