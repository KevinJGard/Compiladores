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

		

		default: return "UNKNOWN";
	}
}
