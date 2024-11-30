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
    'array': 'ARRAY',
}

# literais que são tokens
literals = ['+','-','*','/','=',',',';',':','.','[',']','(',')']


tokens = [
    'ID',
    'NUMBER',
    'WORD',
    'ATRIBUITTION',
    'COMPARING', 
] + list(palavras_reservadas.values())

# expressões regulares para tokens
t_ATRIBUITTION = r':='
t_COMPARING = r'<=|>=|==|!=|<|>'


# removendo as quebras de linha
def t_newline(t):
    r'\n+'
    t.lexer.lineno += len(t.value)

# Expressão regular para identificador
def t_ID(t):
    r'[a-zA-Z_][a-zA-Z_0-9]*'
    t.type = palavras_reservadas.get(t.value, 'ID')
    t.value = str(t.value)
    return t

# Expressão regular para número inteiro
def t_NUMBER(t):
    r'\d+(\.\d+)?'
    t.value = float(t.value) if '.' in t.value else int(t.value)
    return t

def t_WORD(t):
    r'"[A-Za-z0-9\s]*"'
    t.value = str(t.value)
    return t

# ignorar espaços em branco
t_ignore = ' \t'

# caractere de erro
def t_error(t):
    print("Caractere ilegal '%s'" % t.value[0])
    t.lexer.skip(1)

# build the lexer
lexer = lex.lex()

class Token():
    def __init__(self, token):
        self.tipo = tok.type
        self.valor = tok.value
        self.linha = tok.lineno
analisados = list()

# Lendo o arquivo de entrada
def main():
    if len(sys.argv) != 2:
        sys.stderr.write('Usage: %s filename\n' % sys.argv[0])
        raise SystemExit(1)

    filename = sys.argv[1]
    output_filename = 'output_lexer.out'
    with open(filename) as f:
        lexer.input(f.read())
        with open(output_filename, 'w') as out_file:
            for tok in iter(lexer.token, None):
                out_file.write(str(tok) + '\n')
            

if __name__ == '__main__':
    main()


# Exemplo de uso:
# $ python lexic.py input.txt