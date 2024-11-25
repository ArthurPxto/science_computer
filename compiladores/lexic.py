import sys
import ply.lex as lex

# dicionário de palavras reservadas
palavras_reservadas = {
    'const': 'CONST',
    'begin': 'BEGIN',
    'end': 'END',
    'type': 'TYPE',
    'var': 'VAR',
    'integer': 'INTEGER',
    'real': 'REAL',
    'char': 'CHAR',
    'boolean': 'BOOLEAN',
    'of': 'OF',
    'record': 'RECORD',
    'function': 'FUNCTION',
    'procedure': 'PROCEDURE',
    'while': 'WHILE',
    'do': 'DO',
    'if': 'IF',
    'then': 'THEN',
    'for': 'FOR',
    'write': 'WRITE',
    'read': 'READ',
    'to': 'TO',
    'else': 'ELSE',
    'false': 'FALSE',
    'true': 'TRUE',
    'and': 'AND',
    'or': 'OR',
}

#Lista de tokens aceitos pelo analisador léxico
tokens = [
    'IDENTIFIER', # Identificador
    'NUMBER', # Número inteiro
    'PLUS', # Soma +
    'MINUS', # Subtração -
    'TIMES', # Multiplicação *
    'DIVIDE', # Divisão /
    'EQUALS', # Igual ==
    'NOT_EQUALS', # Diferente !=
    'LESS', # Menor que <
    'GREATER', # Maior que >
    'LESS_EQUALS', # Menor ou igual <=
    'GREATER_EQUALS', # Maior ou igual >=
    'ASSIGN', # Atribuição :=
    'ATRIBUITTION', # Atribuição =
    'LPAREN', # Parenteses esquerdo (
    'RPAREN', # Parenteses direito )
    'LBRACE', # Chave esquerda {
    'RBRACE', # Chave direita }
    'LBRAKET', # Colchete esquerdo [
    'RBRAKET', # Colchete direito ]
    'SEMICOLON', # Ponto e vírgula ;
    'COMMA', # Vírgula ,
    'COLON', # Dois pontos :
    'DOT', # Ponto .
    'DOUBLE_DOT', # Dois pontos :
    'BRAKELINE', # Quebra de linha \n
]+ list(palavras_reservadas.values())

# Expressões regulares para tokens simples
t_PLUS = r'\+'
t_MINUS = r'-'
t_TIMES = r'\*'
t_DIVIDE = r'/'
t_EQUALS = r'=='
t_NOT_EQUALS = r'!='
t_LESS = r'<'
t_GREATER = r'>'
t_LESS_EQUALS = r'<='
t_GREATER_EQUALS = r'>='
t_ASSIGN = r':='
t_LPAREN = r'\('
t_RPAREN = r'\)'
t_LBRACE = r'\{'
t_RBRACE = r'\}'
t_LBRAKET = r'\['
t_RBRAKET = r'\]'
t_SEMICOLON = r';'
t_COMMA = r','
t_COLON = r':'
t_DOT = r'\.'
t_DOUBLE_DOT = r':'
t_BRAKELINE = r'\n'
t_ATRIBUITTION = r'='

# removendo as quebras de linha
def t_newline(t):
    r'\n+'
    t.lexer.lineno += len(t.value)

# Expressão regular para identificador
def t_IDENTIFIER(t):
    r'[a-zA-Z_][a-zA-Z_0-9]*'
    t.type = palavras_reservadas.get(t.value, 'IDENTIFIER')
    return t

# Expressão regular para número inteiro
def t_NUMBER(t):
    r'\d+'
    t.value = int(t.value)
    return t

# ignorar espaços em branco
t_ignore = ' \t'

# caractere de erro
def t_error(t):
    print("Caractere ilegal '%s'" % t.value[0])
    t.lexer.skip(1)

# build the lexer
lexer = lex.lex()

# Lendo o arquivo de entrada
def main():
    if len(sys.argv) != 2:
        sys.stderr.write('Usage: %s filename\n' % sys.argv[0])
        raise SystemExit(1)

    filename = sys.argv[1]
    with open(filename) as f:
        lexer.input(f.read())
        for tok in iter(lexer.token, None):
            print(tok)

if __name__ == '__main__':
    main()


# Exemplo de uso:
# $ python lexic.py input.txt