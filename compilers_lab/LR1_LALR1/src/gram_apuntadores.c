#include "gramatica.h"

/**
 * Para mejorar la legibilidad de la tabla de acción.
 * Macros para representar las acciones.
 */
#define X 0         // Error
#define S(j) (j)    // Shift a estado j
#define R(k) (-(k)) // Reduce usando la regla k
#define A ACEPTAR   // 9999
#define N (-1)      // Transición no definida

/**
 * Diccionario de símbolos terminales.
 */
static const char *const TERMINALES[NUM_TERMINALES] = {
    "*", "id", "=", "$"
};

/**
 * Diccionario de símbolos no terminales.
 */
static const char *const NO_TERMINALES[NUM_NO_TERMINALES] = {
    "S", "L", "R"
};

/**
 * Reglas de producción de la gramática.
 * El primer número es el índice del no terminal izquierdo.
 * El segundo número es el tamaño del lado derecho.
 */
static const Regla REGLAS[6] = {
    /*  1 */ { 0, 1, "S' -> S" },
    /*  2 */ { 0, 3, "S -> L = R" },
    /*  3 */ { 0, 1, "S -> R" },
    /*  4 */ { 1, 2, "L -> * R" },
    /*  5 */ { 1, 1, "L -> id" },
    /*  6 */ { 2, 1, "R -> L" }
};

/**
 * Gramática completa.
 * Los espacios vacíos se definen como X en la tabla de acción y N en la tabla de ir_a.
 */
const Gramatica GRAM_APUNTADORES = {
    .nombre = "Gramática con apuntadores",
    .fin_entrada = 3,     // $
    .terminales = TERMINALES,
    .no_terminales = NO_TERMINALES,
    .reglas = REGLAS,
    .num_reglas = 6,
    .accion = {
        /*  0 */ { S(1), S(5), X, X },
        /*  1 */ { S(1), S(5), X, X },
        /*  2 */ { X, X, S(8), R(6) },
        /*  3 */ { X, X, X, R(3) },
        /*  4 */ { X, X, X, A },
        /*  5 */ { X, X, R(5), R(5) },
        /*  6 */ { X, X, R(6), R(6) },
        /*  7 */ { X, X, R(4), R(4) },
        /*  8 */ { S(1), S(5), X, X },
        /*  9 */ { X, X, X, R(2) }
    },
    .ir_a = {
        /* 0 */ { 4, 2, 3 },
        /* 1 */ { N, 6, 7 },
        /* 2 */ { N, N, N },
        /* 3 */ { N, N, N },
        /* 4 */ { N, N, N },
        /* 5 */ { N, N, N },
        /* 6 */ { N, N, N },
        /* 7 */ { N, N, N },
        /* 8 */ { N, 6, 9 },
        /* 9 */ { N, N, N }
    }
};