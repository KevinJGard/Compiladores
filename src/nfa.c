#include <stdlib.h>
#include "nfa.h"

/* 
   B

    */
nfa regex_to_nfa(regex r) {
    nfa n;
    n.transitions    = NULL;
    n.trans_count    = 0;
    n.trans_capacity = 0;
    n.state_count    = 0;
    n.start          = 0;
    n.accept         = 0;
    n.error          = 1;   /* TODO(B): poner 0 cuando este implementado */

    (void)r;   /* silencia el warning de parametro sin usar */

    return n;
}

/* Libera la memoria reservada por regex_to_nfa. */
void nfa_free(nfa *n) {
    free(n->transitions);
    n->transitions = NULL;
    n->trans_count = 0;
    n->trans_capacity = 0;
}