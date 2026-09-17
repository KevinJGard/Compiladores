#ifndef SUBSET_CONSTRUCTION_H
#define SUBSET_CONSTRUCTION_H

#include "dfa.h"
#include "nfa.h"

// Firmas de las funciones de construcción de subconjuntos (subset construction) para convertir un NFA a un DFA
state epsilon_closure(nfa n, state t);
state move_nfa(nfa n, state t, char a);

// Función principal para construir el DFA a partir del NFA
dfa build_dfa(nfa n);

#endif