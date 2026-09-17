#include <stdlib.h>
#include "subset_construction.h"


/* Move(T, a): estados alcanzables desde cualquier estado de T
   consumiendo exactamente el simbolo a. Ignora epsilon. Devuelve un estado nuevo y el que llama lo libera. */
state move_nfa(nfa n, state t, char a) {
    state R = state_create(n.state_count);
    for (int i = 0; i < n.trans_count; i++) {
        transition tr = n.transitions[i];
        if (tr.symbol == a && state_contains(&t, tr.from)) {
            state_add(&R, tr.to);
        }
    }
    return R;
}

/* epsilon-Closure(T): T mas todos los estados alcanzables por
   transiciones epsilon en cadena. Usa una pila de estados
    */
state epsilon_closure(nfa n, state t) {
    state C = state_clone(&t);

    /* Pila simple: a lo sumo hay state_count estados por revisar. */
    int *pila = malloc(n.state_count * sizeof(int));
    int top = 0;

    /* Inicializar la pila con todos los estados que ya estan en t. */
    for (int s = 0; s < n.state_count; s++) {
        if (state_contains(&t, s)) {
            pila[top++] = s;
        }
    }

    while (top > 0) {
        int u = pila[--top];
        for (int i = 0; i < n.trans_count; i++) {
            transition tr = n.transitions[i];
            if (tr.from == u && tr.symbol == EPSILON &&
                !state_contains(&C, tr.to)) {
                state_add(&C, tr.to);
                pila[top++] = tr.to;
            }
        }
    }

    free(pila);
    return C;
}
