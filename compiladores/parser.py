import ply.yacc as yacc
import sys
# Importa o lexer que foi definido anteriormente
from lexic import tokens, lexer

# Regras gramaticais

# Regra inicial
def p_program(p):
    '''program :  declarations block'''
    p[0] = ('program', p[1], p[2]) 

# Regra para bloco
def p_block(p):
    '''block : BEGIN command list_com END'''
    p[0] = ('block', p[2], p[3])

# Regra para declarações
def p_declarations(p):
    '''declarations : def_const type var routine'''
    p[0] = ('declarations', p[1], p[2], p[3], p[4])

# Regra para rotinas
def p_def_routine(p):
    """def_routine : ROTINA def_routine
                  | """
    if len(p) > 1:
        p[0] = ('def_routine', p[1], p[2])
    else: 
        p[0] = ('def_routine', None)

def p_routine(p):
    """routine : FUNCTION ID param_routine ':' type_data block_routine
              | PROCEDURE ID param_routine block_routine"""
    if p[1] == 'function':
        p[0] = ('function', p[2], p[3], p[5], p[6])
    else:
        p[0] = ('procedure', p[2], p[3], p[4])

def p_param_routine(p):
    """param_routine : '(' fields ')'
                    | """
    if len(p) > 1:
        p[0] = ('parametrs', p[2])
    else:
        p[0] = []

# Regra para parametros gerais
def p_parametrs(p):
    """parametrs : ID name
                 | NUMBER
                 | FALSE
                 | TRUE"""
    if len(p) == 3:
        p[0] = ('parametrs', 'id', p[1], p[2])
    else:
        p[0] = ('parametrs', 'literal', p[1])

def p_list_param(p):
    """list_param : parametrs ',' list_param
                   | parametrs
                   | """
    if len(p) == 2:
        p[0] = [p[1]]
    elif len(p) == 3:
        p[0] = [p[1]] + p[3]
    else:
        p[0] = []

def p_block_routine(p):
    '''block_routine : var block'''
    p[0] = ('block_routine', p[1], p[2])

# Regra para constantes
def p_def_const(p):
    '''def_const : const def_const 
                 | '''
    if len(p) > 1:
        p[0] = ('def_const', p[1], p[2])
    else:
        p[0] = ('def_const', None)

def p_const(p):
    '''const : CONST ID '=' NUMBER ';' '''
    p[0] = ('const', p[2], p[4])

def p_const_value(p):
    """const_value : WORD
                   | exp_const"""
    p[0] = ('const_value', p[1])

# Regra para tipos 
def p_type(p):
    '''type : TYPE ID '=' type_data ';' '''
    p[0] = ('type', p[2], p[4])

# Regra para tipos de dados
def p_type_data(p):
    """type_data : INTEGER
                 | REAL
                 | CHAR
                 | BOOLEAN
                 | ARRAY '[' NUMBER ']' OF type_data
                 | RECORD fields END
                 | ID"""
    if len(p) == 2:
        p[0] = ('type_data', p[1])
    elif p[1] == 'array':   
        p[0] = ('array', p[3], p[6])
    elif p[1] == 'record':
        p[0] = ('record', p[2])
    else:
        p[0] = ('simple_type', p[1])

def p_def_type(p):
    """def_type : type def_type
                 | """
    if len(p) > 1:
        p[0] = ('def_type', p[1], p[2])
    else:
        p[0] = ('def_type', None)
# Regra para comandos
def p_command(p):
    """command : ID name atribuittion
               | WHILE com_exp DO com_block
               | IF com_exp THEN com_block else_block
               | FOR com_for DO com_block
               | WRITE const_value
               | READ ID name"""
    if p[1] == 'while':
        p[0] = ('while', p[2], p[4])
    elif p[1] == 'if':
        p[0] = ('if', p[2], p[4], p[5])
    elif p[1] == 'for':
        p[0] = ('for', p[2], p[4])
    elif p[1] == 'write':
        p[0] = ('write', p[2])
    elif p[1] == 'read':
        p[0] = ('read', p[2])
    else:
        p[0] = ('atribuittion', p[1], p[3])

def p_com_for(p):
    """com_for : ID atribuittion parametrs TO parametrs"""
    p[0] = ('com_for', p[1], p[3], p[5])

def p_else_block(p):
    """else_block : ELSE com_block
                  | """
    if len(p) > 1:
        p[0] = ('else_block', p[2])
    else:
        p[0] = None

def p_atribuition(p):
    """atribuition : ID ATRIBUITTION exp"""
    p[0] = ('atribuition', p[2])

def p_name(p):
    """name : '.' ID name
            | '[' parametrs ']' 
            | '(' list_param ')'
            | """
    if len(p) == 1:
        p[0] = None
    else:
        p[0] = ('name', p[1], p[2])

def p_list_com(p):
    """list_com : ';' command list_com
                 | """
    if len(p) > 1:
        p[0] = [p[2]] + p[3]
    else:
        p[0] = []

# Regra para lista de ID
def p_id_list(p):
    '''id_list : ',' ID id_list
               | '''
    if len(p) > 1:
        p[0] = [p[2]] + p[3]
    else:
        p[0] = []

# Regra para campos
def p_fields(p):
    '''fields : field fields_list'''
    p[0] = ('fields', [p[1]] +  p[2])

def p_fields_list(p):
    """fields_list : ';' field fields_list
                    | """
    if len(p) > 1:
        p[0] = [p[2]] + p[3]
    else:
        p[0] = []

def p_field(p):
    '''field : ID id_list ':' tipo_data'''
    p[0] = ('field', [p[1]] + p[2], p[4])

# regra para espresões
def p_assignment(p):
    '''assignment : IDENTIFIER ASSIGN expression'''
    p[0] = ('assign', p[1], p[3])

def var(p):
    """var : var_value var
               | """
    if len(p) > 1:
        p[0] = ('var', p[1], p[2])
    else:
        p[0] = ('var', None)

def p_var_value(p):
    """ var_value : var field ';' """
    p[0] = ('var_value', p[2])

def p_expression(p):
    '''expression : expression PLUS term
                 | expression MINUS term
                 | term'''
    if len(p) == 2:
        p[0] = p[1]
    else:
        p[0] = ('expression', p[1], p[2], p[3])

def p_term(p):
    '''term : term TIMES factor
           | term DIVIDE factor
           | factor'''
    if len(p) == 2:
        p[0] = p[1]
    else:
        p[0] = ('term', p[1], p[2], p[3])

def p_factor(p):
    '''factor : NUMBER
             | IDENTIFIER
             | LPAREN expression RPAREN'''
    if len(p) == 2:
        p[0] = p[1]
    else:
        p[0] = p[2]  # expressão dentro dos parênteses

def p_id_list(p):
    """id_list : ',' ID id_list
                | """
    if len(p) > 1:
        p[0] = [p[2]] + p[3]
    else:
        p[0] = []

def p_com_block(p):
    """com_block : block
               | command"""
    p[0] = ('com_block', p[1])

def p_exp(p):
    """exp : parametrs exp_case1
            | '(' parametrs exp_case2"""
    if len(p) == 3:
        p[0] = ('exp', p[1], p[2])
    elif len(p) == 4:
        p[0] = ('exp_group', p[2], p[3])

def p_exp_case1(p):
    """exp_case1 : math_op exp
              | logic_param logic_exp
              | """
    if len(p) == 1:
        p[0] = None
    elif len(p) == 3:
        p[0] = ('exp_case1', p[1], p[2])

def p_logic_exp(p):
    """logic_exp : logic_op exp
                  | """
    if len(p) == 1:
        p[0] = None
    else:
        p[0] = ('logic_op', p[1], p[2])

def p_logic_param(p):
    """logic_param : comp_op parametrs
                    | """
    if len(p) == 1:
        p[0] = None
    else:
        p[0] = ('logic_param', p[1], p[2])

def p_exp_case2(p):
    """exp_case2: math_op exp ')'
              | logic_param logic_op exp ')'"""
    if len(p) == 4:
        p[0] = ('exp_mat_group', p[1], p[2], p[3])
    elif len(p) == 5:
        p[0] = ('exp_logic_group', p[1], p[2], p[3])

def p_exp_const(p):
    """exp_const : parametrs exp_const_case1
                 | '(' parametrs math_op exp_const ')'"""
    if len(p) == 3:
        p[0] = ('exp_const', p[1], p[2])
    elif len(p) == 6:
        p[0] = ('exp_const_group', p[2], p[3], p[4])

def p_exp_const_l(p):
    """exp_const_l : math_op exp_const
                   | """
    if len(p) == 1:
        p[0] = None
    else:
        p[0] = ('exp_const_l', p[1], p[2])

def p_exp_com(p):
    """exp_com : parametrs logic_param exp_com_l
               | '(' parametrs logic_param logic_op exp_com ')'"""
    if len(p) == 4:
        p[0] = ('exp_com', p[1], p[2], p[3])
    elif len(p) == 7:
        p[0] = ('exp_com_group', p[2], p[3], p[4], p[5])


def p_logic_op(p):
    """logic_op : AND
                 | OR"""
    p[0] = p[1]

def p_comp_op(p):
    """comp_op : '>'
               | '<'
               | COMPARING"""
    p[0] = p[1]

def p_math_op(p):
    """OP_MAT : '+'
              | '-'
              | '*'
              | '/'"""
    p[0] = p[1]

def p_exp_com_l(p):
    """exp_com_l: logic_op exp_com
                 | """
    if len(p) == 1:
        p[0] = None
    else:
        p[0] = ('exp_com_l', p[1], p[2])



def p_empty(p):
    '''empty :'''
    pass


# Erro de sintaxe
def p_error(p):
    if p:
        print(f"Erro de sintaxe próximo a '{p.value}' na linha {p.lineno}.")
    else:
        print("Erro de sintaxe na estrutura, ou no final do código.")


# Construir o parser
parser = yacc.yacc()

# Função para processar o texto
def parse_input(text):
    return parser.parse(text)

# Função principal para testar o parser
def main():
    # Lendo o arquivo de entrada
    if len(sys.argv) != 2:
        sys.stderr.write('Usage: %s filename\n' % sys.argv[0])
        raise SystemExit(1)

    filename = sys.argv[1]
    with open(filename) as f:
        input_text = f.read()

    # Analisando o texto
    result = parse_input(input_text)
    print("Resultado da análise sintática:")
    print(result)

if __name__ == '__main__':
    main()
