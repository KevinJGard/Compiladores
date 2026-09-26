#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "dfa.h"

// Prototipo de tu función
dfa* reachable_dfa(const dfa *automaton);

/*
 * TEST 1: DFA con un estado inalcanzable.
 * - Estados originales: 3 (0, 1, 2)
 * - Alcanzables: 0 -> 1 con 'a'.
 * - Inalcanzables: Estados 2.
 * - Esperado: 2 estados finales (0 y 1).
 */
void test_unreachable(void) {
    printf("\n=== Executing test 1: Unreachable state ===\n");
    dfa original;
    dfa_init(&original);
    original.state_count = 3;

    // Inicializar transiciones en -1
    for (int i = 0; i < 3; i++) {
        for (int c = 0; c < ALPHABET_SIZE; c++) {
            original.table[i][c] = -1;
        }
        original.states[i] = state_create(10); // bitset dummy
    }

    // Transiciones componente alcanzable
    original.table[0]['a'] = 1;
    original.table[1]['b'] = 1;

    // Transiciones inalcanzable
    original.table[2]['a'] = 2;

    // Ejecutar filtro de alcanzabilidad
    dfa *filtered = reachable_dfa(&original);
    assert(filtered != NULL);

    // Verificaciones
    assert(filtered->state_count == 2);
    assert(filtered->table[0]['a'] == 1);
    assert(filtered->table[1]['b'] == 1);
    assert(filtered->table[0]['b'] == -1);

    printf("-> Test 1 PASO exitosamente (Estados reducidos de 3 a %d).\n", filtered->state_count);

    // Liberar memoria
    dfa_free(&original);
    dfa_free(filtered);
    free(filtered);
}

/*
 * TEST 2: DFA con todos sus estados alcanzables.
 * - Estados originales: 3 (0, 1, 2)
 * - Estado 0: va a 1 con 'a'.
 * - Estado 1: va a 2 con 'a'.
 * - Estado 2: tiene auto-bucle con 'b'.
 * - Esperado: 3 estados finales (0, 1, 2).
 */
void test_reachable(void) {
    printf("\n=== Ejecutando Test 2: Autobucle y arista hacia q0 ===\n");
    dfa original;
    dfa_init(&original);
    original.state_count = 3;

    for (int i = 0; i < 3; i++) {
        for (int c = 0; c < ALPHABET_SIZE; c++) {
            original.table[i][c] = -1;
        }
        original.states[i] = state_create(10);
    }

    // Ciclo alcanzable entre 0 y 1
    original.table[0]['a'] = 1;
    original.table[1]['a'] = 2;
    original.table[2]['b'] = 2; // autobucle


    dfa *filtered = reachable_dfa(&original);
    assert(filtered != NULL);

    // Verificaciones
    assert(filtered->state_count == 3);
    assert(filtered->table[0]['a'] == 1);
    assert(filtered->table[1]['a'] == 2);
    assert(filtered->table[2]['b'] == 2);

    printf("-> Test 2 PASO exitosamente (Estados reducidos de 3 a %d).\n", filtered->state_count);

    dfa_free(&original);
    dfa_free(filtered);
    free(filtered);
}

int main(void) {
    printf("INICIANDO SUITE DE PRUEBAS DE ALCANZABILIDAD (ROL A)\n");
    test_unreachable();
    test_reachable();
    printf("\nTODAS LAS PRUEBAS UNITARIAS PASARON CORRECTAMENTE.\n");
    return 0;
}
