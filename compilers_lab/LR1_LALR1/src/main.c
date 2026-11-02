#include <stdio.h>
#include "gramatica.h"
#include "ast.h"
#include "motor.h"

/**
 * Alías de los tokens para las cadenas de prueba
 */
#define TOK_ASTERISCO 0
#define TOK_ID 1
#define TOK_IGUAL 2

/**
 * Función para probar una cadena de tokens.
 * Llama al motor de análisis sintáctico y muestra el resultado.
 */
void probar_cadena(const char *nombre, const int *tokens, char lexemas[][LEX_MAX], int n) {
    printf("\n");
    printf("Probando cadena: %s\n", nombre);
    Nodo *ast = NULL;
    ErrorSintactico err;

    int r = motor_analizar(&GRAM_APUNTADORES, tokens, lexemas, n, 0, &ast, &err);

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
    
    /* Pruebas Válidas */
    // Prueba 1: id = id
    const int t1[] = { TOK_ID, TOK_IGUAL, TOK_ID };
    char l1[][LEX_MAX] = { "a", "", "b" };

    // Prueba 2: * id = id
    const int t2[] = { TOK_ASTERISCO, TOK_ID, TOK_IGUAL, TOK_ID };
    char l2[][LEX_MAX] = { "", "ptr", "", "val" };

    // Prueba 3: * * id = * id
    const int t3[] = { TOK_ASTERISCO, TOK_ASTERISCO, TOK_ID, TOK_IGUAL, TOK_ASTERISCO, TOK_ID };
    char l3[][LEX_MAX] = { "", "", "ptr2", "", "", "ptr1" };

    // Prueba 4: id
    const int t4[] = { TOK_ID };
    char l4[][LEX_MAX] = { "x" };

    // Prueba 5: * id
    const int t5[] = { TOK_ASTERISCO, TOK_ID };
    char l5[][LEX_MAX] = { "", "p" };

    /* Pruebas Inválidas */
    // Prueba 6: id =
    const int t6[] = { TOK_ID, TOK_IGUAL };
    char l6[][LEX_MAX] = { "var", "" };

    // Prueba 7: * = id
    const int t7[] = { TOK_ASTERISCO, TOK_IGUAL, TOK_ID };
    char l7[][LEX_MAX] = { "", "", "x" };

    // Prueba 8: id id
    const int t8[] = { TOK_ID, TOK_ID };
    char l8[][LEX_MAX] = { "a", "b" };

    // Prueba 9: = id
    const int t9[] = { TOK_IGUAL, TOK_ID };
    char l9[][LEX_MAX] = { "", "c" };

    // Prueba 10: id = *
    const int t10[] = { TOK_ID, TOK_IGUAL, TOK_ASTERISCO };
    char l10[][LEX_MAX] = { "x", "", "" };

    probar_cadena("Cadena 1: id = id", t1, l1, sizeof(t1)/sizeof(t1[0]));
    probar_cadena("Cadena 2: * id = id", t2, l2, sizeof(t2)/sizeof(t2[0]));
    probar_cadena("Cadena 3: * * id = * id", t3, l3, sizeof(t3)/sizeof(t3[0]));
    probar_cadena("Cadena 4: id", t4, l4, sizeof(t4)/sizeof(t4[0]));
    probar_cadena("Cadena 5: * id", t5, l5, sizeof(t5)/sizeof(t5[0]));
    probar_cadena("Cadena 6: id =", t6, l6, sizeof(t6)/sizeof(t6[0]));
    probar_cadena("Cadena 7: * = id", t7, l7, sizeof(t7)/sizeof(t7[0]));
    probar_cadena("Cadena 8: id id", t8, l8, sizeof(t8)/sizeof(t8[0]));
    probar_cadena("Cadena 9: = id", t9, l9, sizeof(t9)/sizeof(t9[0]));
    probar_cadena("Cadena 10: id = *", t10, l10, sizeof(t10)/sizeof(t10[0]));

    return 0;
}