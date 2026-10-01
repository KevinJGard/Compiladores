#include <stdio.h>

#include "scanner.h"

extern int yylex(void);
extern char *yytext;

int main(void)
{
	int token;

	while ((token = yylex()) != TOK_EOF) {
		printf("[%s:%s]\n", scanner_token_name(token), yytext);
	}

	return 0;
}

/* Tabla de nombres  */
const char *scanner_token_name(int token)
{
	switch (token) {
		case TOK_EOF: return "EOF";
		case TOK_ERROR: return "ERROR";

		/*  palabras reservadas */
		case TOK_KW_BIG_BANG: return "KW_BIG_BANG";
		case TOK_KW_MUERTE_TERMICA: return "KW_MUERTE_TERMICA";

		case TOK_KW_AGUJERO_NEGRO: return "KW_AGUJERO_NEGRO";
		case TOK_KW_PLANETA: return "KW_PLANETA";
		case TOK_KW_ESTRELLA: return "KW_ESTRELLA";

		case TOK_KW_ROCOSO: return "KW_ROCOSO";
		case TOK_KW_GASEOSO: return "KW_GASEOSO";
		case TOK_KW_NEBULOSA: return "KW_NEBULOSA";
		case TOK_KW_PULSAR: return "KW_PULSAR";
		case TOK_KW_VACIO: return "KW_VACIO";

		case TOK_KW_ECLIPSE: return "KW_ECLIPSE";
		case TOK_KW_DESPEJE: return "KW_DESPEJE";
		case TOK_KW_ORBITA: return "KW_ORBITA";
		case TOK_KW_CONSTELACION: return "KW_CONSTELACION";
		case TOK_KW_GALAXIA: return "KW_GALAXIA";
		case TOK_KW_EN: return "KW_EN";
		case TOK_KW_SUPERNOVA: return "KW_SUPERNOVA";
		case TOK_KW_COLISION: return "KW_COLISION";
		case TOK_KW_COMETA: return "KW_COMETA";

		case TOK_KW_LUMINOSO: return "KW_LUMINOSO";
		case TOK_KW_OSCURO: return "KW_OSCURO";

		case TOK_FN_RADIAR: return "FN_RADIAR";
		case TOK_FN_OBSERVAR: return "FN_OBSERVAR";
	        case TOK_FLOAT_LITERAL: return "FLOAT_LITERAL";
	        case TOK_CHAR_LITERAL: return "CHAR_LITERAL";
	        case TOK_INT_LITERAL: return "INT_LITERAL";
	        case TOK_STRING_LITERAL: return "STRING_LITERAL";
	        case TOK_IDENTIFIER: return "IDENTIFIER";

		/* Operadores y delimitadores */
		case TOK_OP_CONVERGENCIA: return "OP_CONVERGENCIA";
		case TOK_OP_SUPERPOSICION: return "OP_SUPERPOSICION";
		case TOK_OP_INTERFERENCIA: return "OP_INTERFERENCIA";
		case TOK_OP_OSCURIDAD: return "OP_OSCURIDAD";
		case TOK_OP_EXPANSION: return "OP_EXPANSION";
		case TOK_OP_CONTRACCION: return "OP_CONTRACCION";
		case TOK_OP_EXPANSION_ASSIGN: return "OP_EXPANSION_ASSIGN";
		case TOK_OP_CONTRACCION_ASSIGN: return "OP_CONTRACCION_ASSIGN";

		case TOK_INC: return "INC";
		case TOK_DEC: return "DEC";
		case TOK_PLUS_ASSIGN: return "PLUS_ASSIGN";
		case TOK_MINUS_ASSIGN: return "MINUS_ASSIGN";
		case TOK_MUL_ASSIGN: return "MUL_ASSIGN";
		case TOK_DIV_ASSIGN: return "DIV_ASSIGN";
		case TOK_MOD_ASSIGN: return "MOD_ASSIGN";
		case TOK_ASSIGN: return "ASSIGN";

		case TOK_EQ: return "EQ";
		case TOK_NEQ: return "NEQ";
		case TOK_LT: return "LT";
		case TOK_LE: return "LE";
		case TOK_GT: return "GT";
		case TOK_GE: return "GE";

		case TOK_AND: return "AND";
		case TOK_OR: return "OR";
		case TOK_NOT: return "NOT";

		case TOK_PLUS: return "PLUS";
		case TOK_MINUS: return "MINUS";
		case TOK_MUL: return "MUL";
		case TOK_DIV: return "DIV";
		case TOK_MOD: return "MOD";

		case TOK_LPAREN: return "LPAREN";
		case TOK_RPAREN: return "RPAREN";
		case TOK_LBRACE: return "LBRACE";
		case TOK_RBRACE: return "RBRACE";
		case TOK_LBRACKET: return "LBRACKET";
		case TOK_RBRACKET: return "RBRACKET";
		case TOK_COMMA: return "COMMA";
		case TOK_SEMICOLON: return "SEMICOLON";

		default: return "UNKNOWN";
	}
}
