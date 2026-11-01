#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

/** 
 * Crea un nuevo nodo vacío y lo inicializa con valores predeterminados.
 */
static Nodo *nodo_nuevo(void) {
    Nodo *n = (Nodo *)calloc(1, sizeof(Nodo));
    if (!n) {
        fprintf(stderr, "Sin memoria\n");
        exit(EXIT_FAILURE);
    }
    return n;
}

/** 
 * Construye un nodo terminal 
 * Se usa en el SHIFT.
 */
Nodo *nodo_terminal(const char *nombre, const char *lexema) {
    Nodo *n = nodo_nuevo();
    n->etiqueta = nombre;

    if (lexema) {
        strncpy(n->lexema, lexema, LEX_MAX - 1);
        n->lexema[LEX_MAX - 1] = '\0';
    }
    return n;
}

/** 
 * Construye un nodo no terminal. Se usa en el REDUCE.
 * Crea un nodo padre y le asigna como hijos los nodos extraídos de la pila.
 */
Nodo *nodo_no_terminal(const char *nombre, int num_regla, const char *regla_txt,
                       Nodo *const *hijos, int cuantos) {
    Nodo *n = nodo_nuevo();
    n->etiqueta = nombre;
    n->regla_txt = regla_txt;
    n->num_regla = num_regla;
    n->n_hijos = cuantos;

    if (cuantos > 0) {
        n->hijos = (Nodo **)malloc((size_t)cuantos * sizeof(Nodo *));
        if (!n->hijos) {
            fprintf(stderr, "Sin memoria\n");
            exit(EXIT_FAILURE);
        }
        for (int i = 0; i < cuantos; i++) {
            n->hijos[i] = hijos[i];
        }
    }
    return n;
}

/** 
 * Imprime el árbol de sintaxis abstracta de manera recursiva.
 */
void nodo_imprimir(const Nodo *n, int nivel) {
    // Imprime sangría según el nivel del nodo
    for (int i = 0; i < nivel; i++) {
        printf("  ");
    }

    if (n->regla_txt) {
        // Nodo no terminal
        printf("%s   [r%d]\n", n->regla_txt, n->num_regla);
    } else if (n->lexema[0]) {
        // Nodo terminal con lexema
        printf("%s (%s)\n", n->etiqueta, n->lexema);
    } else {
        // Terminal simple
        printf("%s\n", n->etiqueta);
    }

    for (int i = 0; i < n->n_hijos; i++) {
        nodo_imprimir(n->hijos[i], nivel + 1);
    }
}

void nodo_liberar(Nodo *n) {
    if (!n) {
        return;
    }

    for (int i = 0; i < n->n_hijos; i++) {
        nodo_liberar(n->hijos[i]);
    }

    if (n->hijos) {
        free(n->hijos);
    }
    free(n);
}