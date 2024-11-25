%%
int {printf("inteiro: %s\n", yytext);}
real {printf("real: %s\n", yytext);}
if  {printf("if: %s\n", yytext);}
while {printf("while: %s\n", yytext);}
num {printf("num: %s\n", yytext);}
id {printf("id: %s\n", yytext);}
":"     {printf("dois pontos: %s\n", yytext);}
";"     {printf("ponto e virgula \n");}
","     {printf("virgula: %s\n", yytext);}
"+" | "-" | "*" | "/" {printf("operador matemático: %s\n", yytext);}
"<" | ">" | "<=" | ">=" | "==" | "!=" | ":=" {printf("operador relacional: %s\n", yytext);}
"("     {printf("abre parenteses: %s\n", yytext);}
")"     {printf("fecha parenteses: %s\n", yytext);}
"{"     {printf("abre chaves: %s\n", yytext);}
"}"     {printf("fecha chaves: %s\n", yytext);}
"&"     {printf("vazio: %s\n", yytext);}
%%

/*

    inicial - PROG

    não terminais
    (:)
    (;)
    (,)
    (int)
    (real)
    (if)
    (while)
    (id)
    (> | >= | < | <= | == | !=| :=)
    (+ | - | * | /)
    (() {})
    (num)
    (&)
*/
