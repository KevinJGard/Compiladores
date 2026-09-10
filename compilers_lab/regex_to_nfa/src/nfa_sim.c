#include <stdlib.h>
#include <string.h>
#include "nfa.h"

/* ============================================================
   MODULO C - Simulador del NFA

   Implementacion de la simulacion de AFN mediante
   conjuntos de estados representados como arreglos de enteros.
   ============================================================ */

/* Calcula la cerradura epsilon de un conjunto de estados in-place.
   Itera sobre las transiciones hasta que no se agreguen nuevos
   estados al conjunto. */
static void epsilon_closure(nfa n, int *set) {
    int changed;
    /* El ciclo do-while garantiza que se propaguen las cerraduras en cadena */ 
    do {
        /* Usamos esta variable para detectar si se agregaron nuevos estados */
        changed = 0;
        for (int i = 0; i < n.trans_count; i++) {
            /* Si la transicion es epsilon, verificamos si debemos agregar el estado de destino */
            if (n.transitions[i].symbol == EPSILON) {
                int u = n.transitions[i].from;
                int v = n.transitions[i].to;
                
                /* Si el estado de origen esta activo y el de destino no lo esta, lo activamos */
                if (set[u] && !set[v]) {
                    set[v] = 1;
                    changed = 1; /* Marcamos que hubo un cambio, es decir, se agregó un nuevo estado */
                }
            }
        }
    } while (changed);
}

/* Calcula el subconjunto de estados alcanzables al consumir un
   caracter especifico 'c'. Escribe el resultado en 'next'. */
static void move_states(nfa n, const int *current, int *next, char c) {
    /* Inicializamos el conjunto de estados destino */
    memset(next, 0, n.state_count * sizeof(int));

    /* Iteramos sobre todas las transiciones del NFA */
    for (int i = 0; i < n.trans_count; i++) {
        if (n.transitions[i].symbol == c) {
            int u = n.transitions[i].from;
            int v = n.transitions[i].to;
            
            /* Si el estado de origen es uno de los estados actuales */
            if (current[u]) {
                next[v] = 1; /* Activamos el estado de destino en el conjunto 'next' */
            }
        }
    }
}

/* Simula el NFA sobre una cadena por CONJUNTO DE ESTADOS
   (no por backtracking):

     1. current = epsilon-closure({ n.start })
     2. por cada caracter c de la cadena:
            current = epsilon-closure(move(current, c))
     3. acepta si y solo si n.accept esta en current

   Devuelve 1 si acepta, 0 si no. */
int match_nfa(nfa n, const char *str, size_t len) {
    /* Verificamos que el NFA sea valido */
    if (n.error != 0 || n.state_count == 0) {
        return 0; 
    }

    /* Reservamos memoria dinamica para representar los conjuntos de estados, la memoria estarà acotada por el numero de estados del NFA. */
    /* Usamos calloc para que todo el arreglo inicie en 0 (false) */
    int *current = calloc(n.state_count, sizeof(int));
    int *next    = calloc(n.state_count, sizeof(int));

    if (!current || !next) {
        free(current);
        free(next);
        return 0; /* Error de memoria */
    }

    /* Primero calculamos la cerradura epsilon del estado inicial */
    current[n.start] = 1; 
    epsilon_closure(n, current);

    /* Segundo, iteramos sobre cada caracter de la cadena de entrada */
    for (size_t i = 0; i < len; i++) {
        char c = str[i];
        
        /* Calculamos el conjunto de estados alcanzables al consumir el caracter 'c' */
        move_states(n, current, next, c);
        
        /* Calculamos la cerradura epsilon del nuevo conjunto 'next' */
        epsilon_closure(n, next);
        
        /* Intercambiamos los punteros: 'next' pasa a ser 'current' 
           para la siguiente iteracion del ciclo.
         */
        int *temp = current;
        current = next;
        next = temp;
    }

    /* Por ultimo, verificamos si el estado de aceptación esta en el conjunto de estados finales */
    /* Si current[n.accept] es 1, significa que el estado de aceptación fue alcanzado */
    int result = current[n.accept];

    /* Al final de nuestra función, liberamos la memoria dinámica que habíamos reservado para los conjuntos de estados.*/
    free(current);
    free(next);

    /* Devolvemos el resultado de la simulación */
    return result;
}