#ifndef NFA_H
#define NFA_H
#include <stddef.h>
#include "regex.h"
#define EPSILON '\0' /* marca de transicion epsilon */

typedef struct {
  int from; /* indice del estado origen */
  int to; /* indice del estado destino */
  char symbol; /* EPSILON si es transicion epsilon */
} transition;

typedef struct {
  transition *transitions;
  int trans_count;
  int trans_capacity;
  int state_count; /* los estados son 0 .. state_count-1 */
  int start; /* estado inicial */
  int accept; /* estado de aceptacion (Thompson garantiza uno solo) */
  int error; /* 0 si se construyo bien */
} nfa;

nfa regex_to_nfa(regex r);
int match_nfa(nfa n, const char *str, size_t len);
void nfa_free(nfa *n);
#endif
