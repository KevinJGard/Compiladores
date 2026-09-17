#include <stdio.h>
#include <stdlib.h>
#include "nfa.h"
#include "dfa.h"
#include "subset_construction.h"

//Imprime las transiciones del NFA
void print_nfa(nfa n) {
    printf("NFA:\n");
    for (int i = 0; i < n.trans_count; i++) {
        char symbol = n.transitions[i].symbol;
        if (symbol == EPSILON) {
            printf("Transición: %d -ε-> %d\n", n.transitions[i].from, n.transitions[i].to);
        } else {
            printf("Transición: %d -%c-> %d\n", n.transitions[i].from, symbol, n.transitions[i].to);
        }
    }
}

int main() {
    transition transitions[] = {
        {0, 1, 'a'},
        {1, 2, 'a'},
        {1, 2, 'b'},
        {1, 0, 'b'},
        {2, 3, 'a'},
        {3, 2, 'a'},
        {3, 0, EPSILON}
    };
    nfa my_nfa;
    my_nfa.transitions = transitions;
    my_nfa.trans_count = sizeof(transitions) / sizeof(transitions[0]);
    my_nfa.state_count = 4;
    my_nfa.start = 0;
    my_nfa.accept = 0;
    
    printf("Automata Finito No Determinista (NFA):\n");
    printf("Estado inicial: %d\n", my_nfa.start);
    printf("Estado de aceptación: %d\n", my_nfa.accept);
    print_nfa(my_nfa);

    dfa my_dfa = build_dfa(my_nfa);

    printf("\nAutomata Finito Determinista (DFA):\n");
    char alphabet[ALPHABET_SIZE];
    dfa_get_alphabet(my_nfa, alphabet);
    dfa_print_table(&my_dfa, alphabet);

    dfa_free(&my_dfa);
}