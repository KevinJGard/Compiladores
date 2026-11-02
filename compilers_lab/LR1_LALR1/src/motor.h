#ifndef MOTOR_H
#define MOTOR_H

#include "gramatica.h"
#include "ast.h"

/**
 * Estructura para almacenar información sobre un error sintáctico.
 */
typedef struct {
    int pos;                // Posición del token donde ocurrió el error
    int estado;             // Estado en el tope de la pila al momento del error
    const char *token;
    char esperados[512];    // Lista de tokens validos en el estado actual
} ErrorSintactico;

/**
 * Función principal que recibe la gramática, la secuencia de tokens y lexemas, y devuelve:
 * 1 si es aceptada y llena el AST,
 * 0 si es rechazada y llena la estructura de error, 
 * -1 si hay un fallo interno.
 */
int motor_analizar(const Gramatica *g, const int *tokens, char lexemas[][LEX_MAX], int n,
                   int traza, Nodo **ast, ErrorSintactico *err);

#endif