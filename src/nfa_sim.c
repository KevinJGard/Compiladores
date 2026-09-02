#include <stdlib.h>
#include "nfa.h"

/* ============================================================
   MODULO C - Simulador del NFA

   ESTE ARCHIVO ES UN STUB. Sirve para que el proyecto compile
   mientras C lo implementa. C: borra los TODO y escribe aqui.

   Pendientes:
     [ ] epsilon-closure
     [ ] move
     [ ] match_nfa
   ============================================================ */

/* Calcula la cerradura epsilon de un conjunto de estados.
   El conjunto se representa como un arreglo de enteros de tamano
   n.state_count donde set[i] != 0 significa "el estado i esta dentro".
   TODO(C): implementar. */
/* static void epsilon_closure(nfa n, int *set); */

/* Simula el NFA sobre una cadena por CONJUNTO DE ESTADOS
   (no por backtracking):

     1. current = epsilon-closure({ n.start })
     2. por cada caracter c de la cadena:
            current = epsilon-closure(move(current, c))
     3. acepta si y solo si n.accept esta en current

   Devuelve 1 si acepta, 0 si no. */
int match_nfa(nfa n, const char *str, size_t len) {
    (void)n;
    (void)str;
    (void)len;

    return 0;   /* TODO(C): implementar */
}