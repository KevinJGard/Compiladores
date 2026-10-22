#include <stdio.h>
#include "gramatica.h"
#include "ast.h"
#include "motor.h"

/**
 * Alías de los tokens para las cadenas de prueba
 */
#define TOK_BIG_BANG 0
#define TOK_MUERTE_TERMICA 1
#define TOK_PLANETA 2
#define TOK_ESTRELLA 3
#define TOK_ORBITA 4
#define TOK_ECLIPSE 5
#define TOK_ID 6
#define TOK_IGUAL 7
#define TOK_PUNTO_Y_COMA 8
#define TOK_LLAVE_IZQ 9
#define TOK_LLAVE_DER 10
#define TOK_PAREN_IZQ 11
#define TOK_PAREN_DER 12
#define TOK_DESPEJE 13
#define TOK_ROCOSO 14
#define TOK_GASEOSO 15
#define TOK_NEBULOSA 16
#define TOK_SUPERPOSICION 17
#define TOK_INTERFERENCIA 18
#define TOK_CONVERGENCIA 19
#define TOK_OSCURIDAD 20
#define TOK_NUM 21

/**
 * Función para probar una cadena de tokens.
 * Llama al motor de análisis sintáctico y muestra el resultado.
 */
void probar_cadena(const char *nombre, const int *tokens, char lexemas[][LEX_MAX], int n) {
    printf("Probando cadena: %s\n", nombre);
    Nodo *ast = NULL;
    ErrorSintactico err;

    int r = motor_analizar(&GRAM_COSMOLANG, tokens, lexemas, n, 0, &ast, &err);

    printf("\n");
    if (r == 1) {
        printf("Resultado: ACEPTADA\n");
        printf("AST:\n");
        nodo_imprimir(ast, 1);
        nodo_liberar(ast);
    } else if (r == 0) {
        printf("Resultado: RECHAZADA\n");
        printf("Error sintactico en el token #%d ('%s'), estado %d.\n", err.pos + 1, err.token, err.estado);
        printf("Se esperaba: %s\n", err.esperados);
    } else {
        printf("Fallo interno del motor (tabla inconsistente).\n");
    }
}

int main(void) {
    /**
     * Cadenas de prueba con sus tokens y lexemas correspondientes.
     * 5 Válidas y 5 inválidas.
     */

    // Prueba 1: Declaracion de una estrella
    const int t1[] = { TOK_BIG_BANG, TOK_ESTRELLA, TOK_ID, TOK_IGUAL, TOK_NUM, TOK_PUNTO_Y_COMA, TOK_MUERTE_TERMICA };
    char l1[][LEX_MAX] = { "", "", "Sirio", "", "100", "", "" };

    // Prueba 2: Declaracion de un planeta rocoso con superposicion e interferencia
    const int t2[] = { TOK_BIG_BANG, TOK_PLANETA, TOK_ROCOSO, TOK_ID, TOK_IGUAL, TOK_ID, TOK_SUPERPOSICION, TOK_NUM, TOK_INTERFERENCIA, TOK_ID, TOK_PUNTO_Y_COMA, TOK_MUERTE_TERMICA };
    char l2[][LEX_MAX] = { "", "", "", "Marte", "", "masa", "", "42", "", "gravedad", "", "" };

    // Prueba 3: Declaracion de una orbita con estrella y convergencia
    const int t3[] = { TOK_BIG_BANG, TOK_ORBITA, TOK_ID, TOK_LLAVE_IZQ, TOK_ESTRELLA, TOK_ID, TOK_IGUAL, TOK_OSCURIDAD, TOK_PAREN_IZQ, TOK_ID, TOK_CONVERGENCIA, TOK_NUM, TOK_PAREN_DER, TOK_PUNTO_Y_COMA, TOK_LLAVE_DER, TOK_MUERTE_TERMICA };
    char l3[][LEX_MAX] = { "", "", "Luna", "", "", "Sol", "", "", "", "x", "", "10", "", "", "", "" };

    // Prueba 4: Declaracion de un eclipse sin despeje
    const int t4[] = { TOK_BIG_BANG, TOK_ECLIPSE, TOK_PAREN_IZQ, TOK_ID, TOK_PAREN_DER, TOK_LLAVE_IZQ, TOK_ESTRELLA, TOK_ID, TOK_IGUAL, TOK_NUM, TOK_PUNTO_Y_COMA, TOK_LLAVE_DER, TOK_MUERTE_TERMICA };
    char l4[][LEX_MAX] = { "", "", "", "condicion", "", "", "", "EnanaBlanca", "", "5", "", "", "" };

    // Prueba 5: Declaracion de un eclipse con despeje
    const int t5[] = { TOK_BIG_BANG, TOK_ECLIPSE, TOK_PAREN_IZQ, TOK_ID, TOK_SUPERPOSICION, TOK_NUM, TOK_PAREN_DER, TOK_LLAVE_IZQ, TOK_PLANETA, TOK_GASEOSO, TOK_ID, TOK_IGUAL, TOK_NUM, TOK_PUNTO_Y_COMA, TOK_LLAVE_DER, TOK_DESPEJE, TOK_LLAVE_IZQ, TOK_ESTRELLA, TOK_ID, TOK_IGUAL, TOK_ID, TOK_PUNTO_Y_COMA, TOK_LLAVE_DER, TOK_MUERTE_TERMICA };
    char l5[][LEX_MAX] = { "", "", "", "a", "", "1", "", "", "", "", "Jupiter", "", "90", "", "", "", "", "", "Sol", "", "b", "", "", "" };

    // Prueba 6: Declaracion de una estrella sin big_bang
    const int t6[] = { TOK_ESTRELLA, TOK_ID, TOK_IGUAL, TOK_NUM, TOK_PUNTO_Y_COMA, TOK_MUERTE_TERMICA };
    char l6[][LEX_MAX] = { "", "Vega", "", "20", "", "" };

    // Prueba 7: Cuerpo vacío
    const int t7[] = { TOK_BIG_BANG, TOK_MUERTE_TERMICA };
    char l7[][LEX_MAX] = { "", "" };

    // Prueba 8: Declaracion de una estrella sin punto y coma
    const int t8[] = { TOK_BIG_BANG, TOK_ESTRELLA, TOK_ID, TOK_IGUAL, TOK_NUM, TOK_MUERTE_TERMICA };
    char l8[][LEX_MAX] = { "", "", "Polaris", "", "50", "" };

    // Prueba 9: Declaracion de un planeta sin tipo
    const int t9[] = { TOK_BIG_BANG, TOK_PLANETA, TOK_ID, TOK_IGUAL, TOK_NUM, TOK_PUNTO_Y_COMA, TOK_MUERTE_TERMICA };
    char l9[][LEX_MAX] = { "", "", "Tierra", "", "1", "", "" };

    // Prueba 10: Declaracion de un eclipse pero sin llave de apertura
    const int t10[] = { TOK_BIG_BANG, TOK_ECLIPSE, TOK_PAREN_IZQ, TOK_ID, TOK_PAREN_DER, TOK_ESTRELLA, TOK_ID, TOK_IGUAL, TOK_NUM, TOK_PUNTO_Y_COMA, TOK_MUERTE_TERMICA };
    char l10[][LEX_MAX] = { "", "", "", "var", "", "", "Sol", "", "0", "", "" };

    probar_cadena("Cadena 1: big_bang estrella id = num ; muerte_termica", t1, l1, sizeof(t1) / sizeof(t1[0]));
    probar_cadena("Cadena 2: big_bang planeta rocoso id = id superposicion num interferencia id ; muerte_termica", t2, l2, sizeof(t2) / sizeof(t2[0]));
    probar_cadena("Cadena 3: big_bang orbita id { estrella id = oscuridad ( id convergencia num ) ; } muerte_termica", t3, l3, sizeof(t3) / sizeof(t3[0]));
    probar_cadena("Cadena 4: big_bang eclipse ( id ) { estrella id = num ; } muerte_termica", t4, l4, sizeof(t4) / sizeof(t4[0]));
    probar_cadena("Cadena 5: big_bang eclipse ( id superposicion num ) { planeta gaseoso id = num ; } despeje { estrella id = id ; } muerte_termica", t5, l5, sizeof(t5) / sizeof(t5[0]));
    probar_cadena("Cadena 6: estrella id = num ; muerte_termica", t6, l6, sizeof(t6) / sizeof(t6[0]));
    probar_cadena("Cadena 7: big_bang muerte_termica", t7, l7, sizeof(t7) / sizeof(t7[0]));
    probar_cadena("Cadena 8: big_bang estrella id = num muerte_termica", t8, l8, sizeof(t8) / sizeof(t8[0]));
    probar_cadena("Cadena 9: big_bang planeta id = num ; muerte_termica", t9, l9, sizeof(t9) / sizeof(t9[0]));
    probar_cadena("Cadena 10: big_bang eclipse ( id ) estrella id = num ; muerte_termica", t10, l10, sizeof(t10) / sizeof(t10[0]));

    return 0;
}