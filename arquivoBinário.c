#include <stdio.h>
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

void menu() {
    //system( "clear" );
    printf( "******** Agenda ********" );
    printf( "\n(1) Inserir" );
    printf( "\n(2) Mostrar todos" );
    printf( "\n(3) Recriar arquivo" );
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
                getchar();
                break;
            case '3':
                recria_arquivo( arq, nome_arq );
                printf( "Arquivo recriado com sucesso...!" );
                getchar();
                break;
        }               
    } while ( ch != '0' && (int)ch != 27 );
           
    fclose( arq );
   
    return 0;
}