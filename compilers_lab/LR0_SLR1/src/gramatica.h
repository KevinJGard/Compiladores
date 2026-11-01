#ifndef GRAMATICA_H
#define GRAMATICA_H

#define NUM_ESTADOS 53
#define NUM_TERMINALES 23
#define NUM_NO_TERMINALES 8
#define ACEPTAR 9999            // Código para aceptar la entrada

/**
 * Estructura que representa una regla de producción en la gramática.
 */
typedef struct {
    int lhs;            // Índice del símbolo no terminal en el lado izquierdo de la producción
    int largo;          // Número de símbolos en el lado derecho de la producción
    const char *texto;
} Regla;

/**
 * Estructura que representa una gramática.
 * Almacena los diccionarios de símbolos y las matrices de acción e ir_a.
 */
typedef struct {
    const char *nombre;
    int fin_entrada;
    const char *const *terminales;
    const char *const *no_terminales;
    const Regla *reglas;
    int num_reglas;
    const int accion[NUM_ESTADOS][NUM_TERMINALES];
    const int ir_a[NUM_ESTADOS][NUM_NO_TERMINALES];
} Gramatica;

extern const Gramatica GRAM_COSMOLANG;

#endif