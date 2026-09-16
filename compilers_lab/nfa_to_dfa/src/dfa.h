#ifndef DFA_H
#define DFA_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <nfa.h>


#define MAX_DFA_STATES 128
#define ALPHABET_SIZE 256


// Simulamos bitset con arreglos de uint64
typedef struct {
  uint64_t *words;
  int num_states;
  int num_words;
  bool accepts;
} state;

typedef struct {
  state states[MAX_DFA_STATES]; // Quiero hacer el arreglo acá
  int state_count;
  // Se asume que el estado inicial siempre estará en el estado cero del arreglo states.
  int table[MAX_DFA_STATES][ALPHABET_SIZE];
  int error;         // 0 si se construyó bien

} dfa;

// Operaciones del bitset (para Rol A y Rol B)
state state_create(int num_nfa_states);
void  state_free(state *s);
state state_clone(const state *src);
void  state_add(state *s, int nfa_state);
bool  state_contains(const state *s, int nfa_state);
void  state_union(state *dest, const state *src);
bool  state_equals(const state *a, const state *b);
bool  state_is_empty(const state *s);

// Para crear el dfa o liberarlo
void dfa_init(dfa *d);
void dfa_free(dfa *d);
int  dfa_find_or_add_state(dfa *d, state s, int nfa_accept_state);
void dfa_print_table(const dfa *d, const char *alphabet);
int  dfa_get_alphabet(nfa n, char *alphabet);

#endif
