#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dfa.h"
#include "nfa.h"

// Crea un subconjunto vacío reservando palabras de 64 bits
state state_create(int num_nfa_states) {
    state s;
    s.num_states = num_nfa_states;
    s.num_words  = (num_nfa_states + 63) / 64;
    s.accepts    = false;
    s.words      = calloc(s.num_words, sizeof(uint64_t));
    return s;
}

// Libera la memoria del bitset
void state_free(state *s) {
    if (s && s->words) {
        free(s->words);
        s->words = NULL;
    }
}

// Clona un estado duplicando su memoria dinámica
state state_clone(const state *s) {
    state c = state_create(s->num_states);
    c.accepts = s->accepts;
    if (s->words && c.words) {
        memcpy(c.words, s->words, s->num_words * sizeof(uint64_t));
    }
    return c;
}

// Enciende el bit correspondiente a nfa_state
void state_add(state *s, int nfa_state) {
    if (!s || !s->words || nfa_state < 0 || nfa_state >= s->num_states) return;
    int word_idx = nfa_state / 64;
    int bit_idx  = nfa_state % 64;
    s->words[word_idx] |= (1ULL << bit_idx);
}

// Consulta si un estado del NFA pertenece al conjunto
bool state_contains(const state *s, int nfa_state) {
    if (!s || !s->words || nfa_state < 0 || nfa_state >= s->num_states) return false;
    int word_idx = nfa_state / 64;
    int bit_idx  = nfa_state % 64;
    return (s->words[word_idx] & (1ULL << bit_idx)) != 0ULL;
}

// Realiza la unión de conjuntos (estados, en este caso). La unión es guardada en el primer estado
void state_union(state *dest, const state *src) {
    if (!dest || !src || !dest->words || !src->words) return;
    int words_to_op = dest->num_words < src->num_words ? dest->num_words : src->num_words; // Aunque deberían tener el mismo número de palabras
    for (int i = 0; i < words_to_op; i++) {
        dest->words[i] |= src->words[i];
    }
    if (src->accepts) {
        dest->accepts = true;
    }
}

// Compara si dos subconjuntos son iguales
bool state_equals(const state *a, const state *b) {
    if (!a || !b || !a->words || !b->words) return false;
    if (a->num_states != b->num_states || a->num_words != b->num_words) return false;
    return memcmp(a->words, b->words, a->num_words * sizeof(uint64_t)) == 0;
}

// Verifica si el subconjunto está totalmente vacío
bool state_is_empty(const state *s) {
    if (!s || !s->words) return true;
    for (int i = 0; i < s->num_words; i++) {
        if (s->words[i] != 0ULL) return false;
    }
    return true;
}

// ============================================================
// 2. GESTIÓN DEL DFA
// ============================================================

// Inicializa el autómata determinista y limpia la matriz de transiciones
void dfa_init(dfa *d) {
    d->state_count = 0;
    d->error       = 0;
    for (int i = 0; i < MAX_DFA_STATES; i++) {
        d->states[i].words = NULL;
        d->states[i].accepts = false;
        d->states[i].num_states = 0;
        d->states[i].num_words = 0;
        for (int c = 0; c < ALPHABET_SIZE; c++) {
            d->table[i][c] = -1; // -1 indica ausencia de transición
        }
    }
}

// Libera todos los bitsets almacenados en el DFA
void dfa_free(dfa *d) {
    if (!d) return;
    for (int i = 0; i < d->state_count; i++) {
        state_free(&d->states[i]);
    }
    d->state_count = 0;
}

// Registra un subconjunto o retorna su índice si ya fue descubierto
int dfa_find_or_add_state(dfa *d, state s, int nfa_accept_state) {
    // 1. Si ya existe en la lista, liberar la copia y devolver el ID existente
    for (int i = 0; i < d->state_count; i++) {
        if (state_equals(&d->states[i], &s)) {
            state_free(&s);
            return i;
        }
    }

    // 2. Validación de capacidad máxima
    if (d->state_count >= MAX_DFA_STATES) {
        fprintf(stderr, "Error: Se excedio el limite MAX_DFA_STATES (%d)\n", MAX_DFA_STATES);
        d->error = 1;
        state_free(&s);
        return -1;
    }

    // 3. Registrar nuevo estado
    int new_id = d->state_count;
    d->states[new_id] = s;
    d->states[new_id].accepts = state_contains(&s, nfa_accept_state);

    for (int c = 0; c < ALPHABET_SIZE; c++) {
        d->table[new_id][c] = -1;
    }

    d->state_count++;
    return new_id;
}

// Imprime la tabla de transiciones
// Se utiliza un arreglo de chars para imprimir las 256 columnas
void dfa_print_table(const dfa *d, const char *alphabet) {
    if (!d || d->error != 0) {
        printf("[DFA Inválido o con error de construcción]\n");
        return;
    }

    int alpha_len = strlen(alphabet);

    printf("\n-- Transiciones --\n");
    printf("---------------------------------------\n");
    printf(" Estado |");
    for (int i = 0; i < alpha_len; i++) {
        printf("  '%c'  |", alphabet[i]);
    }
    printf(" Acepta | Subconjunto NFA\n");
    printf("---------------------------------------\n");

    for (int i = 0; i < d->state_count; i++) {
        // Estado inicial con '->'
        if (i == 0) {
            printf(" ->q%-3d |", i);
        } else {
            printf("   q%-3d |", i);
        }

        // Transiciones para cada símbolo del alfabeto
        for (int j = 0; j < alpha_len; j++) {
            unsigned char c = (unsigned char)alphabet[j];
            int dest = d->table[i][c];
            if (dest != -1) {
                printf("  q%-3d |", dest);
            } else {
                printf("   -   |");
            }
        }

        // Estado de aceptación
        printf("   %-4s  | { ", d->states[i].accepts ? "SI" : "NO");

	// Subconjuntos de los estados del dfa presentes en este estado nfa
        bool first = true;
        for (int s = 0; s < d->states[i].num_states; s++) {
            if (state_contains(&d->states[i], s)) {
                if (!first) printf(", ");
                printf("%d", s);
                first = false;
            }
        }
        printf(" }\n");
    }
    printf("---------------------------------------\n\n");
}

int dfa_get_alphabet(nfa n, char *alphabet) {
    bool seen[256] = {false};
    int count = 0;
    for (int i = 0; i < n.trans_count; i++) {
        unsigned char c = (unsigned char)n.transitions[i].symbol;
        if (c != EPSILON && !seen[c]) {
            seen[c] = true;
            alphabet[count++] = (char)c;
        }
    }
    alphabet[count] = '\0';
    return count;
}
