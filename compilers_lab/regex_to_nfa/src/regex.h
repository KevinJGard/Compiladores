#ifndef REGEX_H
#define REGEX_H



/* Tipos de token que puede contener una expresion regular. */
typedef enum {
    TOKEN_CHAR,     /* simbolo del alfabeto: a, b, c, ... */
    TOKEN_CONCAT,   /* concatenacion explicita  */
    TOKEN_UNION,    /* |  */
    TOKEN_STAR,     /* *  cero o mas  */
    TOKEN_PLUS,     /* +  una o mas    */
    TOKEN_QUESTION, /* ?  cero o uno */
    TOKEN_LPAREN,   /* (  solo vive durante Shunting-Yard   */
    TOKEN_RPAREN    /* )  nunca llega al postfijo   */
} token_type;

/* Codigos de error que puede devolver parse_regex. */
typedef enum {
    REGEX_OK = 0,
    REGEX_ERR_UNBALANCED,   /* parentesis sin cerrar: "(a|b" */
    REGEX_ERR_BAD_OPERATOR, /* operador sin operando: "*a" */
    REGEX_ERR_EMPTY,        /* expresion vacia */
    REGEX_ERR_MEMORY        /* fallo de malloc             */
} regex_error;

/* Un token individual. */
typedef struct {
    token_type type;
    char value;      
} regex_item;

/* Resultado del parseo: arreglo de tokens en notacion postfija. */
typedef struct {
    regex_item *items;  
    int size;
    regex_error error;   
} regex;

/* Convierte una regex infija en tokens postfijos con
   concatenacion explicita. Revisar siempre el campo .error
   antes de usar .items */
regex parse_regex(const char *regex_str);

/* Libera la memoria reservada por parse_regex. */
void regex_free(regex *r);

#endif /* REGEX_H */ 