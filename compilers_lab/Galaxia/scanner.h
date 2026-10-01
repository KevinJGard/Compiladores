#ifndef SCANNER_H
#define SCANNER_H


typedef enum ScannerToken {
	TOK_EOF = 0,
	TOK_ERROR = 256,

	/* palabras reservadas */

	/* Delimitadores del programa */
	TOK_KW_BIG_BANG,        /* big_bang        inicio del programa */
	TOK_KW_MUERTE_TERMICA,  /* muerte_termica  fin del programa    */

	/* Declaraciones */
	TOK_KW_AGUJERO_NEGRO,   /* agujero_negro   def   */
	TOK_KW_PLANETA,         /* planeta         var   */
	TOK_KW_ESTRELLA,        /* estrella        const */

	/* Tipos de dato */
	TOK_KW_ROCOSO,          /* rocoso   entero  */
	TOK_KW_GASEOSO,         /* gaseoso  flotante*/
	TOK_KW_NEBULOSA,        /* nebulosa cadena  */
	TOK_KW_PULSAR,          /* pulsar   booleano*/
	TOK_KW_VACIO,           /* vacio    void    */

	/* Control de flujo */
	TOK_KW_ECLIPSE,         /* eclipse       if       */
	TOK_KW_DESPEJE,         /* despeje       else     */
	TOK_KW_ORBITA,          /* orbita        while    */
	TOK_KW_CONSTELACION,    /* constelacion  for      */
	TOK_KW_GALAXIA,         /* galaxia       foreach  */
	TOK_KW_EN,              /* en            in       */
	TOK_KW_SUPERNOVA,       /* supernova     return   */
	TOK_KW_COLISION,        /* colision      break    */
	TOK_KW_COMETA,          /* cometa        continue */

	/* Literales booleanos */
	TOK_KW_LUMINOSO,        /* luminoso  true  */
	TOK_KW_OSCURO,          /* oscuro    false */

	/* Funciones nativas */
	TOK_FN_RADIAR,          /* radiar    print */
	TOK_FN_OBSERVAR,        /* observar  input */

	/* identificadores y literales */
	TOK_IDENTIFIER,
	TOK_INT_LITERAL,
	TOK_FLOAT_LITERAL,
	TOK_STRING_LITERAL,
	TOK_CHAR_LITERAL,

	/* Operadores y Delimitadores*/

	/* Operadores de bits, escritos como palabra en CosmoLang. */
	TOK_OP_CONVERGENCIA,    /* convergencia   &  */
	TOK_OP_SUPERPOSICION,   /* superposicion  |  */
	TOK_OP_INTERFERENCIA,   /* interferencia  ^  */
	TOK_OP_OSCURIDAD,       /* oscuridad      ~  */
	TOK_OP_EXPANSION,       /* expansion      << */
	TOK_OP_CONTRACCION,     /* contraccion    >> */
	TOK_OP_EXPANSION_ASSIGN,  /* <<= */
	TOK_OP_CONTRACCION_ASSIGN,/* >>= */

	/* Aritmeticos */
	TOK_PLUS,
	TOK_MINUS,
	TOK_MUL,
	TOK_DIV,
	TOK_MOD,

	/* Compuestos */
	TOK_INC,
	TOK_DEC,
	TOK_PLUS_ASSIGN,
	TOK_MINUS_ASSIGN,
	TOK_MUL_ASSIGN,
	TOK_DIV_ASSIGN,
	TOK_MOD_ASSIGN,
	TOK_ASSIGN,

	/* Relacionales */
	TOK_EQ,
	TOK_NEQ,
	TOK_LT,
	TOK_LE,
	TOK_GT,
	TOK_GE,

	/* Logicos */
	TOK_AND,
	TOK_OR,
	TOK_NOT,

	/* Delimitadores */
	TOK_LPAREN,
	TOK_RPAREN,
	TOK_LBRACE,
	TOK_RBRACE,
	TOK_LBRACKET,
	TOK_RBRACKET,
	TOK_COMMA,
	TOK_SEMICOLON
} ScannerToken;

const char *scanner_token_name(int token);

#endif /* SCANNER_H */
