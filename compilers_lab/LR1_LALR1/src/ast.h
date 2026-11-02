/** 
 * Árbol de sintaxis abstracta 
 * Definición de la estructura de datos y funciones para el AST.
 */
#ifndef AST_H
#define AST_H

#define LEX_MAX 64

typedef struct Nodo {
    const char *etiqueta;   // Nombre del símbolo
    const char *regla_txt;  // Texto de la regla de producción (si es un no terminal)
    int num_regla;
    char lexema[LEX_MAX];
    int n_hijos;   
    struct Nodo **hijos;
} Nodo;

/** Construye un nodo terminal */
Nodo *nodo_terminal(const char *nombre, const char *lexema);

/** Construye un nodo no terminal */
Nodo *nodo_no_terminal(const char *nombre, int num_regla, const char *regla_txt,
                       Nodo *const *hijos, int n);

/** Imprime el árbol de sintaxis abstracta */
void nodo_imprimir(const Nodo *n, int nivel);

/** Libera la memoria de un nodo y sus hijos */
void nodo_liberar(Nodo *n);

#endif