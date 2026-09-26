#ifndef HOPCROFT_H
#define HOPCROFT_H

#include "dfa.h"

/* Representa el conjunto P o la cola W (una lista de subconjuntos de estados) */
typedef struct {
    state items[MAX_DFA_STATES]; // Limitado a la cantidad maxima de estados del DFA
    int size;
} state_list;

/* Funcion principal. Algoritmo de Hopcroft para minimizar un DFA. */
state_list minimize_hopcroft(const dfa *d);

/* Libera la memoria de la lista de particiones */
void list_free(state_list *l);

#endif