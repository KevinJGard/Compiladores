#include "hopcroft.h"
#include <stdlib.h>

/*
 * Funciones auxiliares para el algoritmo de Hopcroft 
 */

/* Retorna la cantidad de estados encendidos en el bitset */
static int state_size(const state *s) {
    int count = 0;
    for (int i = 0; i < s->num_states; i++) {
        if (state_contains(s, i)) count++;
    }
    return count;
}

/* Y1 = Y ∩ X (Interseccion) */
static state state_intersect(const state *y, const state *x) {
    state res = state_create(y->num_states);
    for (int i = 0; i < y->num_states; i++) {
        if (state_contains(y, i) && state_contains(x, i)) {
            state_add(&res, i);
        }
    }
    return res;
}

/* Y2 = Y \ X (Diferencia) */
static state state_difference(const state *y, const state *x) {
    state res = state_create(y->num_states);
    for (int i = 0; i < y->num_states; i++) {
        if (state_contains(y, i) && !state_contains(x, i)) {
            state_add(&res, i);
        }
    }
    return res;
}

/* Extrae y retorna el primer elemento de la lista. Lo usamos para simular una cola y su comportamiento */
static state queue_pop(state_list *q) {
    state res = q->items[0];
    for (int i = 1; i < q->size; i++) {
        q->items[i - 1] = q->items[i];
    }
    q->size--;
    return res;
}

/* Busca un conjunto en la lista. En caso de encontrarlo, retorna su indice o -1 si no esta. */
static int find_in_list(const state_list *l, const state *target) {
    for (int i = 0; i < l->size; i++) {
        if (state_equals(&l->items[i], target)) return i;
    }
    return -1;
}

/* Libera la memoria de la lista de particiones */
void list_free(state_list *l) {
    for (int i = 0; i < l->size; i++) {
        state_free(&l->items[i]);
    }
    l->size = 0;
}



/* Algoritmo de Hopcroft para minimizar un DFA (En base al pseudocódigo de Hopcroft recibido)
 * @param d: DFA a minimizar
 * @return: Lista de particiones finales (cada particion es un conjunto de estados)
*/
state_list minimize_hopcroft(const dfa *d) {
    // Inicializar P (particiones) y W (cola de trabajo)
    state_list P; P.size = 0;
    state_list W; W.size = 0;
    
    // Número de estados en el DFA
    int n = d->state_count;
    
    // Inicializar F y Q \ F
    state F = state_create(n);
    state Q_minus_F = state_create(n);

    for (int i = 0; i < n; i++) {
        if (d->states[i].accepts) { // Si el estado es de aceptación, agregarlo a F
            state_add(&F, i);
        } else {
            state_add(&Q_minus_F, i); // Si no es de aceptación, agregarlo a Q \ F
        }
    }

    // Si F no es vacío, agregarlo a P; si Q \ F no es vacío, agregarlo a P
    if (!state_is_empty(&F))
        P.items[P.size++] = state_clone(&F);
    if (!state_is_empty(&Q_minus_F))
        P.items[P.size++] = state_clone(&Q_minus_F);

    // Si F y Q \ F no son vacíos, agregar ambos a W
    if (!state_is_empty(&F) && !state_is_empty(&Q_minus_F)) {
        W.items[W.size++] = state_clone(&F);
        W.items[W.size++] = state_clone(&Q_minus_F);
    }

    // Mientras W no esté vacío, continuar refinando las particiones
    while (W.size > 0) {
        state A = queue_pop(&W);

        // Para cada símbolo del alfabeto, encontrar el conjunto de estados que transicionan a A
        for (int c = 0; c < ALPHABET_SIZE; c++) { 
            
            // Buscamos todos los estados q en Q que al aplicar la transición con el símbolo c nos lleve a un estado en A, o bien:
            // X = {q in Q | delta(q, c) in A}
            state X = state_create(n);
            for (int q = 0; q < n; q++) {
                int target = d->table[q][c]; // Estado al que transiciona q con el símbolo c
                if (target != -1 && state_contains(&A, target)) { // Si el estado de destino está en A, agregar q a X
                    state_add(&X, q);
                }
            }

            // Si X es vacío, no hay nada que refinar, continuar con el siguiente símbolo
            if (state_is_empty(&X)) {
                state_free(&X);
                continue;
            }

            // Revisar cada Y en la particion P
            // Para cada Y en P, si Y ∩ X != ∅ y Y \ X != ∅, entonces reemplazar Y con Y1 = Y ∩ X y Y2 = Y \ X
            for (int i = 0; i < P.size; i++) {
                state Y = P.items[i];
                
                state Y1 = state_intersect(&Y, &X);
                state Y2 = state_difference(&Y, &X);

                // Si ambos Y1 y Y2 no son vacíos, entonces reemplazar Y con Y1 y Y2 en P
                if (!state_is_empty(&Y1) && !state_is_empty(&Y2)) {
                    int w_idx = find_in_list(&W, &Y); 

                    // P = (P \ {Y}) ∪ {Y1, Y2}
                    state_free(&P.items[i]);
                    P.items[i] = state_clone(&Y1); 
                    P.items[P.size++] = state_clone(&Y2);

                    // Si Y estaba en W, reemplazarlo con Y1 y Y2; si no estaba, agregar el más pequeño de Y1 o Y2 a W
                    if (w_idx != -1) {
                        state_free(&W.items[w_idx]);
                        W.items[w_idx] = state_clone(&Y1);
                        W.items[W.size++] = state_clone(&Y2);
                    } else {
                        if (state_size(&Y1) <= state_size(&Y2)) {
                            W.items[W.size++] = state_clone(&Y1);
                        } else {
                            W.items[W.size++] = state_clone(&Y2);
                        }
                    }
                } 
                
                state_free(&Y1);
                state_free(&Y2);
            }
            state_free(&X);
        }
        state_free(&A); // Liberar A después de procesarlo
    }

    state_free(&F);
    state_free(&Q_minus_F);
    list_free(&W); 
    
    // Devolver la lista de particiones finales para pasar a construir el DFA minimizado
    return P; 
}