#include <stdio.h>
#include <stdlib.h>
#include "dfa.h"
#include "hopcroft.h"
#include "minimize.h"

// Demostración del flujo completo de minimización sobre un DFA de ejemplo:
// primero se descartan los estados inalcanzables, luego se refinan las
// particiones con Hopcroft y finalmente se reconstruye el DFA mínimo.
//
// El ejemplo usa el alfabeto {0,1} e incluye a propósito un estado
// inalcanzable (el 6) y estados equivalentes que deben fusionarse:
//   estados 0..6, inicial 0, aceptación {2,3,4}
//   mínimo esperado: 3 clases -> {0,1}, {2,3,4}, {5}

// Arma el DFA de ejemplo. Cada estado marca su propio índice en el bitset para
// que la columna de subconjuntos muestre qué estados terminan fusionados.
static void build_example_dfa(dfa *d) {
    dfa_init(d);
    int n = 7;
    d->state_count = n;

    for (int i = 0; i < n; i++) {
        d->states[i] = state_create(n);
        state_add(&d->states[i], i);
    }

    d->states[2].accepts = true;
    d->states[3].accepts = true;
    d->states[4].accepts = true;

    d->table[0]['0'] = 1; d->table[0]['1'] = 2;
    d->table[1]['0'] = 0; d->table[1]['1'] = 3;
    d->table[2]['0'] = 4; d->table[2]['1'] = 5;
    d->table[3]['0'] = 4; d->table[3]['1'] = 5;
    d->table[4]['0'] = 4; d->table[4]['1'] = 5;
    d->table[5]['0'] = 5; d->table[5]['1'] = 5;
    d->table[6]['0'] = 6; d->table[6]['1'] = 6; // estado inalcanzable
}

int main(void) {
    const char *alphabet = "01";

    printf("Minimización de un DFA\n");

    dfa original;
    build_example_dfa(&original);
    print_dfa(&original, alphabet);

    // Se eliminan los estados inalcanzables antes de minimizar.
    printf("\nEliminación de estados inalcanzables:\n");
    dfa *reduced = reachable_dfa(&original);
    if (!reduced) {
        fprintf(stderr, "Error: reachable_dfa devolvió NULL.\n");
        dfa_free(&original);
        return 1;
    }

    // Refinamiento de particiones con Hopcroft.
    printf("\nRefinamiento de particiones (Hopcroft):\n");
    state_list partitions = minimize_hopcroft(reduced);
    printf("Clases de equivalencia encontradas: %d\n", partitions.size);

    // Reconstrucción del DFA mínimo a partir de las clases.
    dfa minimized = build_min_dfa(reduced, &partitions);
    print_dfa_min(&minimized, alphabet);

    printf("Estados: %d (original) -> %d (alcanzable) -> %d (mínimo)\n",
           original.state_count, reduced->state_count, minimized.state_count);

    list_free(&partitions);
    dfa_free(reduced);
    free(reduced);
    dfa_free(&minimized);
    dfa_free(&original);

    return 0;
}
