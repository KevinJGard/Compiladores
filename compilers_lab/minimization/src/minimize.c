#include <stdio.h>
#include "minimize.h"

// Busca en qué partición cae el estado original. Hopcroft particiona toda Q,
// así que en la práctica siempre lo encuentra; devuelve -1 por seguridad.
int partition_index_of(const state_list *partitions, int original_state) {
    if (!partitions || original_state < 0) return -1;
    for (int i = 0; i < partitions->size; i++) {
        if (state_contains(&partitions->items[i], original_state)) {
            return i;
        }
    }
    return -1;
}

// Devuelve el estado de menor índice de una clase (su representante), o -1 si
// la clase está vacía.
static int representative_of(const state *class) {
    for (int i = 0; i < class->num_states; i++) {
        if (state_contains(class, i)) {
            return i;
        }
    }
    return -1;
}

// Reconstruye el DFA mínimo. Cada clase de equivalencia se vuelve un estado:
// como los estados de una misma clase son indistinguibles, basta con mirar un
// representante para conocer su aceptación y sus transiciones. La clase del
// estado inicial 0 se numera como estado 0 para no romper el resto del código,
// que asume states[0] como inicial.
dfa build_min_dfa(const dfa *original, const state_list *partitions) {
    dfa min;
    dfa_init(&min);

    if (!original || !partitions || partitions->size == 0) {
        min.error = 1;
        return min;
    }

    int n_classes = partitions->size;

    // Mapa clase -> id nuevo. Empezamos por la clase del estado inicial.
    int class_new_id[MAX_DFA_STATES];
    for (int i = 0; i < n_classes; i++) class_new_id[i] = -1;

    int q0_class = partition_index_of(partitions, 0);
    if (q0_class < 0) {
        min.error = 1;
        return min;
    }

    class_new_id[q0_class] = 0;
    int next_id = 1;
    for (int i = 0; i < n_classes; i++) {
        if (class_new_id[i] == -1) {
            class_new_id[i] = next_id++;
        }
    }

    // Mapa inverso: id nuevo -> clase original.
    int id_class[MAX_DFA_STATES];
    for (int i = 0; i < n_classes; i++) {
        id_class[class_new_id[i]] = i;
    }

    min.state_count = n_classes;

    for (int new_id = 0; new_id < n_classes; new_id++) {
        const state *class = &partitions->items[id_class[new_id]];
        int rep = representative_of(class);

        // El bitset del nuevo estado guarda la unión de los subconjuntos de sus
        // miembros, así la tabla sigue mostrando qué estados se fusionaron.
        state merged = state_create(original->states[rep].num_states);
        for (int q = 0; q < class->num_states; q++) {
            if (state_contains(class, q)) {
                state_union(&merged, &original->states[q]);
            }
        }
        merged.accepts = original->states[rep].accepts;
        min.states[new_id] = merged;

        // La transición de una clase es la clase a la que va su representante.
        for (int c = 0; c < ALPHABET_SIZE; c++) {
            int target = original->table[rep][c];
            if (target != -1) {
                int target_class = partition_index_of(partitions, target);
                min.table[new_id][c] = (target_class != -1)
                                           ? class_new_id[target_class]
                                           : -1;
            } else {
                min.table[new_id][c] = -1;
            }
        }
    }

    return min;
}

void print_dfa(const dfa *d, const char *alphabet) {
    printf("\nDFA antes de minimizar:\n");
    dfa_print_table(d, alphabet);
}

void print_dfa_min(const dfa *d, const char *alphabet) {
    printf("\nDFA después de minimizar:\n");
    dfa_print_table(d, alphabet);
}
