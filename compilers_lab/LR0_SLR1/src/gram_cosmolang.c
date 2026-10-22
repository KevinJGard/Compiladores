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
static const char *const TERMINALES[23] = {
    "big_bang", "muerte_termica", "planeta", "estrella", "orbita", "eclipse", "id", "=", ";", "{", "}",
    "(", ")", "despeje", "rocoso", "gaseoso", "nebulosa", "superposicion", "interferencia",
    "convergencia", "oscuridad", "num", "$"
};

/**
 * Diccionario de símbolos no terminales.
 */
static const char *const NO_TERMINALES[8] = { "P", "Cu", "C", "TI", "E", "T", "U", "V" };

/**
 * Reglas de producción de la gramática.
 * El primer número es el índice del no terminal izquierdo.
 * El segundo número es el tamaño del lado derecho.
 */
static const Regla REGLAS[21] = {
    /*  1 */ { 0, 3, "P -> big_bang Cu muerte_termica" },
    /*  2 */ { 1, 2, "Cu -> Cu C" },
    /*  3 */ { 1, 1, "Cu -> C" },
    /*  4 */ { 2, 6, "C -> planeta TI id = E ;" },
    /*  5 */ { 2, 5, "C -> estrella id = E ;" },
    /*  6 */ { 2, 5, "C -> orbita id { Cu }" },
    /*  7 */ { 2, 7, "C -> eclipse ( E ) { Cu }" },
    /*  8 */ { 2, 11, "C -> eclipse ( E ) { Cu } despeje { Cu }" },
    /*  9 */ { 3, 1, "TI -> rocoso" },
    /* 10 */ { 3, 1, "TI -> gaseoso" },
    /* 11 */ { 3, 1, "TI -> nebulosa" },
    /* 12 */ { 4, 3, "E -> E superposicion T" },
    /* 13 */ { 4, 1, "E -> T" },
    /* 14 */ { 5, 3, "T -> T interferencia U" },
    /* 15 */ { 5, 1, "T -> U" },
    /* 16 */ { 6, 3, "U -> U convergencia V" },
    /* 17 */ { 6, 1, "U -> V" },
    /* 18 */ { 7, 2, "V -> oscuridad V" },
    /* 19 */ { 7, 3, "V -> ( E )" },
    /* 20 */ { 7, 1, "V -> id" },
    /* 21 */ { 7, 1, "V -> num" },
};

/**
 * Gramática completa de CosmoLang.
 * Los espacios vacíos se definen como X en la tabla de acción y N en la tabla de ir_a.
 */
const Gramatica GRAM_COSMOLANG = {
    .nombre = "CosmoLang",
    .fin_entrada = 22,      // $
    .terminales = TERMINALES,
    .no_terminales = NO_TERMINALES,
    .reglas = REGLAS,
    .num_reglas = 21,
    .accion = {
        /*  0 */ { S(2), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X },
        /*  1 */ { X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, A },
        /*  2 */ { X, X, S(5), S(6), S(7), S(8), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X },
        /*  3 */ { X, S(9), S(5), S(6), S(7), S(8), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X },
        /*  4 */ { X, R(3), R(3), R(3), R(3), R(3), X, X, X, X, R(3), X, X, X, X, X, X, X, X, X, X, X, X },
        /*  5 */ { X, X, X, X, X, X, X, X, X, X, X, X, X, X, S(12), S(13), S(14), X, X, X, X, X, X },
        /*  6 */ { X, X, X, X, X, X, S(15), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X },
        /*  7 */ { X, X, X, X, X, X, S(16), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X },
        /*  8 */ { X, X, X, X, X, X, X, X, X, X, X, S(17), X, X, X, X, X, X, X, X, X, X, X },
        /*  9 */ { X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, R(1) },
        /* 10 */ { X, R(2), R(2), R(2), R(2), R(2), X, X, X, X, R(2), X, X, X, X, X, X, X, X, X, X, X, X },
        /* 11 */ { X, X, X, X, X, X, S(18), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X },
        /* 12 */ { X, X, X, X, X, X, R(9), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X },
        /* 13 */ { X, X, X, X, X, X, R(10), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X },
        /* 14 */ { X, X, X, X, X, X, R(11), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X },
        /* 15 */ { X, X, X, X, X, X, X, S(19), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X },
        /* 16 */ { X, X, X, X, X, X, X, X, X, S(20), X, X, X, X, X, X, X, X, X, X, X, X, X },
        /* 17 */ { X, X, X, X, X, X, S(27), X, X, X, X, S(26), X, X, X, X, X, X, X, X, S(25), S(28), X },
        /* 18 */ { X, X, X, X, X, X, X, S(29), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X },
        /* 19 */ { X, X, X, X, X, X, S(27), X, X, X, X, S(26), X, X, X, X, X, X, X, X, S(25), S(28), X },
        /* 20 */ { X, X, S(5), S(6), S(7), S(8), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X },
        /* 21 */ { X, X, X, X, X, X, X, X, X, X, X, X, S(32), X, X, X, X, S(33), X, X, X, X, X },
        /* 22 */ { X, X, X, X, X, X, X, X, R(13), X, X, X, R(13), X, X, X, X, R(13), S(34), X, X, X, X },
        /* 23 */ { X, X, X, X, X, X, X, X, R(15), X, X, X, R(15), X, X, X, X, R(15), R(15), S(35), X, X, X },
        /* 24 */ { X, X, X, X, X, X, X, X, R(17), X, X, X, R(17), X, X, X, X, R(17), R(17), R(17), X, X, X },
        /* 25 */ { X, X, X, X, X, X, S(27), X, X, X, X, S(26), X, X, X, X, X, X, X, X, S(25), S(28), X },
        /* 26 */ { X, X, X, X, X, X, S(27), X, X, X, X, S(26), X, X, X, X, X, X, X, X, S(25), S(28), X },
        /* 27 */ { X, X, X, X, X, X, X, X, R(20), X, X, X, R(20), X, X, X, X, R(20), R(20), R(20), X, X, X },
        /* 28 */ { X, X, X, X, X, X, X, X, R(21), X, X, X, R(21), X, X, X, X, R(21), R(21), R(21), X, X, X },
        /* 29 */ { X, X, X, X, X, X, S(27), X, X, X, X, S(26), X, X, X, X, X, X, X, X, S(25), S(28), X },
        /* 30 */ { X, X, X, X, X, X, X, X, S(39), X, X, X, X, X, X, X, X, S(33), X, X, X, X, X },
        /* 31 */ { X, X, S(5), S(6), S(7), S(8), X, X, X, X, S(40), X, X, X, X, X, X, X, X, X, X, X, X },
        /* 32 */ { X, X, X, X, X, X, X, X, X, S(41), X, X, X, X, X, X, X, X, X, X, X, X, X },
        /* 33 */ { X, X, X, X, X, X, S(27), X, X, X, X, S(26), X, X, X, X, X, X, X, X, S(25), S(28), X },
        /* 34 */ { X, X, X, X, X, X, S(27), X, X, X, X, S(26), X, X, X, X, X, X, X, X, S(25), S(28), X },
        /* 35 */ { X, X, X, X, X, X, S(27), X, X, X, X, S(26), X, X, X, X, X, X, X, X, S(25), S(28), X },
        /* 36 */ { X, X, X, X, X, X, X, X, R(18), X, X, X, R(18), X, X, X, X, R(18), R(18), R(18), X, X, X },
        /* 37 */ { X, X, X, X, X, X, X, X, X, X, X, X, S(45), X, X, X, X, S(33), X, X, X, X, X },
        /* 38 */ { X, X, X, X, X, X, X, X, S(46), X, X, X, X, X, X, X, X, S(33), X, X, X, X, X },
        /* 39 */ { X, R(5), R(5), R(5), R(5), R(5), X, X, X, X, R(5), X, X, X, X, X, X, X, X, X, X, X, X },
        /* 40 */ { X, R(6), R(6), R(6), R(6), R(6), X, X, X, X, R(6), X, X, X, X, X, X, X, X, X, X, X, X },
        /* 41 */ { X, X, S(5), S(6), S(7), S(8), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X },
        /* 42 */ { X, X, X, X, X, X, X, X, R(12), X, X, X, R(12), X, X, X, X, R(12), S(34), X, X, X, X },
        /* 43 */ { X, X, X, X, X, X, X, X, R(14), X, X, X, R(14), X, X, X, X, R(14), R(14), S(35), X, X, X },
        /* 44 */ { X, X, X, X, X, X, X, X, R(16), X, X, X, R(16), X, X, X, X, R(16), R(16), R(16), X, X, X },
        /* 45 */ { X, X, X, X, X, X, X, X, R(19), X, X, X, R(19), X, X, X, X, R(19), R(19), R(19), X, X, X },
        /* 46 */ { X, R(4), R(4), R(4), R(4), R(4), X, X, X, X, R(4), X, X, X, X, X, X, X, X, X, X, X, X },
        /* 47 */ { X, X, S(5), S(6), S(7), S(8), X, X, X, X, S(48), X, X, X, X, X, X, X, X, X, X, X, X },
        /* 48 */ { X, R(7), R(7), R(7), R(7), R(7), X, X, X, X, R(7), X, X, S(49), X, X, X, X, X, X, X, X, X },
        /* 49 */ { X, X, X, X, X, X, X, X, X, S(50), X, X, X, X, X, X, X, X, X, X, X, X, X },
        /* 50 */ { X, X, S(5), S(6), S(7), S(8), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X },
        /* 51 */ { X, X, S(5), S(6), S(7), S(8), X, X, X, X, S(52), X, X, X, X, X, X, X, X, X, X, X, X },
        /* 52 */ { X, R(8), R(8), R(8), R(8), R(8), X, X, X, X, R(8), X, X, X, X, X, X, X, X, X, X, X, X }
    },
    
    .ir_a = {
        /*  0 */ { 1, N, N, N, N, N, N, N },
        /*  1 */ { N, N, N, N, N, N, N, N },
        /*  2 */ { N, 3, 4, N, N, N, N, N },
        /*  3 */ { N, N, 10, N, N, N, N, N },
        /*  4 */ { N, N, N, N, N, N, N, N },
        /*  5 */ { N, N, N, 11, N, N, N, N },
        /*  6 */ { N, N, N, N, N, N, N, N },
        /*  7 */ { N, N, N, N, N, N, N, N },
        /*  8 */ { N, N, N, N, N, N, N, N },
        /*  9 */ { N, N, N, N, N, N, N, N },
        /* 10 */ { N, N, N, N, N, N, N, N },
        /* 11 */ { N, N, N, N, N, N, N, N },
        /* 12 */ { N, N, N, N, N, N, N, N },
        /* 13 */ { N, N, N, N, N, N, N, N },
        /* 14 */ { N, N, N, N, N, N, N, N },
        /* 15 */ { N, N, N, N, N, N, N, N },
        /* 16 */ { N, N, N, N, N, N, N, N },
        /* 17 */ { N, N, N, N, 21, 22, 23, 24 },
        /* 18 */ { N, N, N, N, N, N, N, N },
        /* 19 */ { N, N, N, N, 30, 22, 23, 24 },
        /* 20 */ { N, 31, 4, N, N, N, N, N },
        /* 21 */ { N, N, N, N, N, N, N, N },
        /* 22 */ { N, N, N, N, N, N, N, N },
        /* 23 */ { N, N, N, N, N, N, N, N },
        /* 24 */ { N, N, N, N, N, N, N, N },
        /* 25 */ { N, N, N, N, N, N, N, 36 },
        /* 26 */ { N, N, N, N, 37, 22, 23, 24 },
        /* 27 */ { N, N, N, N, N, N, N, N },
        /* 28 */ { N, N, N, N, N, N, N, N },
        /* 29 */ { N, N, N, N, 38, 22, 23, 24 },
        /* 30 */ { N, N, N, N, N, N, N, N },
        /* 31 */ { N, N, 10, N, N, N, N, N },
        /* 32 */ { N, N, N, N, N, N, N, N },
        /* 33 */ { N, N, N, N, N, 42, 23, 24 },
        /* 34 */ { N, N, N, N, N, N, 43, 24 },
        /* 35 */ { N, N, N, N, N, N, N, 44 },
        /* 36 */ { N, N, N, N, N, N, N, N },
        /* 37 */ { N, N, N, N, N, N, N, N },
        /* 38 */ { N, N, N, N, N, N, N, N },
        /* 39 */ { N, N, N, N, N, N, N, N },
        /* 40 */ { N, N, N, N, N, N, N, N },
        /* 41 */ { N, 47, 4, N, N, N, N, N },
        /* 42 */ { N, N, N, N, N, N, N, N },
        /* 43 */ { N, N, N, N, N, N, N, N },
        /* 44 */ { N, N, N, N, N, N, N, N },
        /* 45 */ { N, N, N, N, N, N, N, N },
        /* 46 */ { N, N, N, N, N, N, N, N },
        /* 47 */ { N, N, 10, N, N, N, N, N },
        /* 48 */ { N, N, N, N, N, N, N, N },
        /* 49 */ { N, N, N, N, N, N, N, N },
        /* 50 */ { N, 51, 4, N, N, N, N, N },
        /* 51 */ { N, N, 10, N, N, N, N, N },
        /* 52 */ { N, N, N, N, N, N, N, N }
    }
};