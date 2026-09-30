%option noyywrap yylineno

%{
#include "scanner.h"
%}

%x COMMENT

ID          [a-zA-Z_][a-zA-Z0-9_]*
INT         [0-9]+
FLOAT       ({INT}\.[0-9]*([eE][+-]?{INT})?|{INT}[eE][+-]?{INT})
WS          [ \t\r\n]+

%%

 /* 
    1. COMENTARIOS Y ESPACIOS  
    (bloque provisional)*/

"meteorito"[ \t]*"/"    { BEGIN(COMMENT); }
<COMMENT>"/"[ \t]*"meteorito"  { BEGIN(INITIAL); }
<COMMENT>\n             { }
<COMMENT>.              { }
<COMMENT><<EOF>>        { return TOK_ERROR; }

"meteorito"[^\n]*       { /* comentario de linea */ }

{WS}                    { /* ignorar espacios */ }

 /* 
    2. PALABRAS RESERVADAS Y FUNCIONES NATIVAS
     */

 /* Delimitadores del programa */
"big_bang"              { return TOK_KW_BIG_BANG; }
"muerte_termica"        { return TOK_KW_MUERTE_TERMICA; }

 /* Declaraciones */
"agujero_negro"         { return TOK_KW_AGUJERO_NEGRO; }
"planeta"               { return TOK_KW_PLANETA; }
"estrella"              { return TOK_KW_ESTRELLA; }

 /* Tipos de dato */
"rocoso"                { return TOK_KW_ROCOSO; }
"gaseoso"               { return TOK_KW_GASEOSO; }
"nebulosa"              { return TOK_KW_NEBULOSA; }
"pulsar"                { return TOK_KW_PULSAR; }
"vacio"                 { return TOK_KW_VACIO; }

 /* Control de flujo */
"eclipse"               { return TOK_KW_ECLIPSE; }
"despeje"               { return TOK_KW_DESPEJE; }
"orbita"                { return TOK_KW_ORBITA; }
"constelacion"          { return TOK_KW_CONSTELACION; }
"galaxia"               { return TOK_KW_GALAXIA; }
"en"                    { return TOK_KW_EN; }
"supernova"             { return TOK_KW_SUPERNOVA; }
"colision"              { return TOK_KW_COLISION; }
"cometa"                { return TOK_KW_COMETA; }

 /* Literales booleanos */
"luminoso"              { return TOK_KW_LUMINOSO; }
"oscuro"                { return TOK_KW_OSCURO; }

 /* Funciones nativas */
"radiar"                { return TOK_FN_RADIAR; }
"observar"              { return TOK_FN_OBSERVAR; }

 /* 
    3. OPERADORES Y DELIMITADORES   
    (bloque provision)

    */

"convergencia"          { return TOK_OP_CONVERGENCIA; }
"superposicion"         { return TOK_OP_SUPERPOSICION; }
"interferencia"         { return TOK_OP_INTERFERENCIA; }
"oscuridad"             { return TOK_OP_OSCURIDAD; }
"expansion"             { return TOK_OP_EXPANSION; }
"contraccion"           { return TOK_OP_CONTRACCION; }

"++"                    { return TOK_INC; }
"--"                    { return TOK_DEC; }
"+="                    { return TOK_PLUS_ASSIGN; }
"-="                    { return TOK_MINUS_ASSIGN; }
"*="                    { return TOK_MUL_ASSIGN; }
"/="                    { return TOK_DIV_ASSIGN; }
"%="                    { return TOK_MOD_ASSIGN; }
"=="                    { return TOK_EQ; }
"!="                    { return TOK_NEQ; }
"<="                    { return TOK_LE; }
">="                    { return TOK_GE; }
"&&"                    { return TOK_AND; }
"||"                    { return TOK_OR; }

"="                     { return TOK_ASSIGN; }
"<"                     { return TOK_LT; }
">"                     { return TOK_GT; }
"!"                     { return TOK_NOT; }
"+"                     { return TOK_PLUS; }
"-"                     { return TOK_MINUS; }
"*"                     { return TOK_MUL; }
"/"                     { return TOK_DIV; }
"%"                     { return TOK_MOD; }

"("                     { return TOK_LPAREN; }
")"                     { return TOK_RPAREN; }
"{"                     { return TOK_LBRACE; }
"}"                     { return TOK_RBRACE; }
"["                     { return TOK_LBRACKET; }
"]"                     { return TOK_RBRACKET; }
","                     { return TOK_COMMA; }
";"                     { return TOK_SEMICOLON; }

 /* 
    4. LITERALES E IDENTIFICADORES   (bloque provisional) */

{FLOAT}                     { return TOK_FLOAT_LITERAL; }
{INT}                       { return TOK_INT_LITERAL; }
\'([^\\\n\']|\\.)\'         { return TOK_CHAR_LITERAL; }
\'([^\\\n\']|\\.)*          { return TOK_ERROR; }
\"([^\\\"\n]|\\.)*\"        { return TOK_STRING_LITERAL; }
\"([^\\\"\n]|\\.)*          { return TOK_ERROR; }
{ID}                        { return TOK_IDENTIFIER; }


 /* 
    5. FIN DE ARCHIVO Y ERRORES */

<<EOF>>                 { return TOK_EOF; }

.                       { return TOK_ERROR; }

%%
