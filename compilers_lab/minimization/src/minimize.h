#ifndef MINIMIZE_H
#define MINIMIZE_H

#include "dfa.h"
#include "hopcroft.h"

// Reconstrucción del DFA mínimo a partir de la partición final de Hopcroft.
// Se asigna un id nuevo a cada clase de equivalencia, se fija el estado
// inicial y los de aceptación, y se llena la nueva tabla de transiciones.

// Índice de la partición que contiene al estado original dado (-1 si no está).
int partition_index_of(const state_list *partitions, int original_state);

// Construye el DFA mínimo desde el DFA original (ya sin estados inalcanzables)
// y la partición P. La clase que contiene al estado inicial 0 recibe el id 0.
dfa build_min_dfa(const dfa *original, const state_list *partitions);

// Imprime la tabla del DFA antes de minimizar.
void print_dfa(const dfa *d, const char *alphabet);

// Imprime la tabla del DFA después de minimizar.
void print_dfa_min(const dfa *d, const char *alphabet);

#endif
