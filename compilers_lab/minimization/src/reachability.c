#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "dfa.h"

dfa* reachable_dfa(const dfa *automaton) {
    if (!automaton || automaton->state_count == 0) {
        return NULL;
    }

    dfa *result = (dfa *)malloc(sizeof(dfa));
    if (!result) return NULL;
    dfa_init(result);

    // Se guardan los nuevos estados, sus correspondientes en el dfa original y viceversa
    int old_to_new[MAX_DFA_STATES];
    int new_to_old[MAX_DFA_STATES];
    for (int i = 0; i < MAX_DFA_STATES; i++) {
        old_to_new[i] = -1;
    }

    bool visited[MAX_DFA_STATES] = {false};
    int stack[MAX_DFA_STATES];
    int top = -1;

    int new_state_count = 0;
    int initial = 0; // El estado inicial siempre es 0

    // Registrar estado inicial
    visited[initial] = true;
    old_to_new[initial] = new_state_count;
    new_to_old[new_state_count] = initial;
    result->states[new_state_count] = state_clone(&automaton->states[initial]);
    new_state_count++;
    stack[++top] = initial;

    // DFS
    while (top >= 0) {
        int u = stack[top--];

        for (int c = 0; c < ALPHABET_SIZE; c++) {
            int neigh = automaton->table[u][c];

            if (neigh >= 0 && neigh < automaton->state_count) {
                if (!visited[neigh]) {
                    visited[neigh] = true;
                    old_to_new[neigh] = new_state_count;
                    new_to_old[new_state_count] = neigh;
                    result->states[new_state_count] = state_clone(&automaton->states[neigh]);
                    new_state_count++;
                    stack[++top] = neigh;
                }
            }
        }
    }

    printf("--- Estados inalcanzables detectados: ---\n");
    int descartados = 0;
    for (int i = 0; i < automaton->state_count; i++) {
      if (!visited[i]) {
        printf("Estado descartado: %d\n", i);
        printf("El estado %d no es alcanzable desde el estado inicial 0.\n", i);
        descartados++;
      }
    }

    if (descartados == 0) {
      printf("El DFA no contiene estados inalcanzables. Se conservan los %d estados.\n", automaton->state_count);
    }

    result->state_count = new_state_count;

    // Inicializar la nueva tabla de transiciones con -1
    for (int i = 0; i < MAX_DFA_STATES; i++) {
        for (int c = 0; c < ALPHABET_SIZE; c++) {
            result->table[i][c] = -1;
        }
    }

    // Se reconstruyen las transiciones con los nuevos id's
    for (int i = 0; i < new_state_count; i++) {
        int os = new_to_old[i];
        for (int c = 0; c < ALPHABET_SIZE; c++) {
            int target = automaton->table[os][c];
            if (target >= 0 && target < automaton->state_count) {
                result->table[i][c] = old_to_new[target];
            }
        }
    }

    return result;
}
