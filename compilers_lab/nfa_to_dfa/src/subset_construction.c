#include <stdio.h>
#include <stdlib.h>
#include "subset_construction.h"

/**
 * Función principal para construir el DFA a partir del NFA
 * @param n: El NFA a convertir
 * @return: El DFA resultante
 */ 
dfa build_dfa(nfa n) {
    dfa d;
    dfa_init(&d); // Inicializa matriz en -1 y conteos en 0

    // Extraemos el alfabeto del NFA para saber sobre qué caracteres iterar
    char alphabet[ALPHABET_SIZE];
    int alpha_len = dfa_get_alphabet(n, alphabet);

    // Obtenemos el estado inicial del NFA y calculamos su clausura epsilon
    state q0_set = state_create(n.state_count);
    state_add(&q0_set, n.start); // Agregamos el estado inicial del NFA
    
    state s0 = epsilon_closure(n, q0_set);
    state_free(&q0_set); // Liberamos la memoria del conjunto temporal {q0}

    // 2 y 3: Qd <- {s0}, Fd <- vacío (dfa_find_or_add_state ya maneja si es de aceptación)
    int s0_id = dfa_find_or_add_state(&d, s0, n.accept);

    // Inicializamos la "Cola" para el algoritmo de construcción del DFA
    // Como máximo habrá MAX_DFA_STATES, un arreglo simple funciona perfecto como cola
    int queue[MAX_DFA_STATES];
    int head = 0, tail = 0;
    
    queue[tail++] = s0_id; // Encolar el índice del estado inicial

    // Mientras haya estados por procesar en la "Cola"
    while (head < tail) {
        
        // Desencolamos el siguiente estado U del DFA para procesar sus transiciones
        int u_id = queue[head++];
        state U = d.states[u_id]; // Obtenemos el subconjunto actual

        // Para cada entrada del alfabeto, calculamos la transición
        for (int i = 0; i < alpha_len; i++) {
            char a = alphabet[i];

            // Calculamos el conjunto de estados alcanzables desde U con el símbolo a y después su clausura epsilon
            state move_res = move_nfa(n, U, a);
            state V = epsilon_closure(n, move_res);
            state_free(&move_res); // Liberamos el paso intermedio de move()

            // Si no encontramos un estado vacío, lo agregamos al DFA y a la cola si es nuevo
            if (!state_is_empty(&V)) {
                
                int prev_count = d.state_count;
                
                // Verificamos si el estado V ya existe en el DFA o lo agregamos, ya lo checa la función dfa_find_or_add_state
                int v_id = dfa_find_or_add_state(&d, V, n.accept);

                // Manejo de error si excedemos MAX_DFA_STATES
                if (v_id == -1) {
                    return d; 
                }

                // Pasamos a "Encolar" el nuevo estado si es que fue agregado (su ID será >= prev_count)
                if (v_id >= prev_count) {
                    queue[tail++] = v_id;
                }

                // Pasamos a agregar la transición en la tabla del DFA
                d.table[u_id][(unsigned char)a] = v_id;
            } else {
                // Si el subconjunto resultante está vacío, no se mete al DFA.
                // Es importante liberar esta memoria aquí porque dfa_find_or_add_state no tiene esa responsabilidad para estados vacíos.
                state_free(&V);
            }
        }
    }

    // Regresamos el DFA construido. Si hubo error, el campo d.error será distinto de 0.
    return d;
}