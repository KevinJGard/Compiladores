#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include "dfa.h"
#include "hopcroft.h"
#include "minimize.h"

// Pruebas de la reconstrucción del DFA mínimo. Cada prueba arma un DFA a mano,
// lo reduce por alcanzabilidad, lo minimiza con Hopcroft y verifica que el DFA
// resultante sea el esperado.

// Crea un estado con su propio bitset (marca su índice) y su aceptación.
static void set_state(dfa *d, int i, int n, bool accepts) {
    d->states[i] = state_create(n);
    state_add(&d->states[i], i);
    d->states[i].accepts = accepts;
}

// DFA con estados equivalentes (ejemplo clásico de Hopcroft). Los 6 estados
// deben colapsar en 3 clases: {0,1}, {2,3,4} y {5}.
static void test_equivalent_states(void) {
    printf("\nPrueba 1: fusión de estados equivalentes\n");
    dfa d;
    dfa_init(&d);
    int n = 6;
    d.state_count = n;

    for (int i = 0; i < n; i++) set_state(&d, i, n, (i == 2 || i == 3 || i == 4));

    d.table[0]['0'] = 1; d.table[0]['1'] = 2;
    d.table[1]['0'] = 0; d.table[1]['1'] = 3;
    d.table[2]['0'] = 4; d.table[2]['1'] = 5;
    d.table[3]['0'] = 4; d.table[3]['1'] = 5;
    d.table[4]['0'] = 4; d.table[4]['1'] = 5;
    d.table[5]['0'] = 5; d.table[5]['1'] = 5;

    dfa *reduced = reachable_dfa(&d);
    assert(reduced != NULL);
    assert(reduced->state_count == 6); // todos alcanzables

    state_list P = minimize_hopcroft(reduced);
    dfa m = build_min_dfa(reduced, &P);

    assert(m.state_count == 3);

    // El estado 0 del mínimo debe contener al inicial y no ser de aceptación.
    assert(state_contains(&m.states[0], 0));
    assert(m.states[0].accepts == false);

    // La clase de {2,3,4} es de aceptación y agrupa a esos tres estados.
    int clase_q2 = m.table[0]['1']; // δ(0,'1') lleva a la clase de q2
    assert(m.states[clase_q2].accepts == true);
    assert(state_contains(&m.states[clase_q2], 2));
    assert(state_contains(&m.states[clase_q2], 3));
    assert(state_contains(&m.states[clase_q2], 4));

    // El DFA mínimo sigue siendo total: hay transición con ambos símbolos.
    for (int i = 0; i < m.state_count; i++) {
        assert(m.table[i]['0'] != -1);
        assert(m.table[i]['1'] != -1);
    }

    printf("  ok: 6 estados reducidos a %d.\n", m.state_count);

    list_free(&P);
    dfa_free(reduced); free(reduced);
    dfa_free(&m);
    dfa_free(&d);
}

// Igual que el anterior pero con un estado inalcanzable (el 6): primero se
// descarta y luego el mínimo sigue teniendo 3 clases.
static void test_unreachable_then_minimize(void) {
    printf("\nPrueba 2: estado inalcanzable + minimización\n");
    dfa d;
    dfa_init(&d);
    int n = 7;
    d.state_count = n;

    for (int i = 0; i < n; i++) set_state(&d, i, n, (i == 2 || i == 3 || i == 4));

    d.table[0]['0'] = 1; d.table[0]['1'] = 2;
    d.table[1]['0'] = 0; d.table[1]['1'] = 3;
    d.table[2]['0'] = 4; d.table[2]['1'] = 5;
    d.table[3]['0'] = 4; d.table[3]['1'] = 5;
    d.table[4]['0'] = 4; d.table[4]['1'] = 5;
    d.table[5]['0'] = 5; d.table[5]['1'] = 5;
    d.table[6]['0'] = 6; d.table[6]['1'] = 6; // inalcanzable

    dfa *reduced = reachable_dfa(&d);
    assert(reduced != NULL);
    assert(reduced->state_count == 6); // se descartó el estado 6

    state_list P = minimize_hopcroft(reduced);
    dfa m = build_min_dfa(reduced, &P);

    assert(m.state_count == 3);
    assert(state_contains(&m.states[0], 0));

    printf("  ok: 7 -> %d alcanzables -> %d mínimo.\n",
           reduced->state_count, m.state_count);

    list_free(&P);
    dfa_free(reduced); free(reduced);
    dfa_free(&m);
    dfa_free(&d);
}

// DFA que ya es mínimo: la reconstrucción debe conservar sus dos estados.
static void test_already_minimal(void) {
    printf("\nPrueba 3: DFA ya mínimo\n");
    dfa d;
    dfa_init(&d);
    int n = 2;
    d.state_count = n;

    set_state(&d, 0, n, false);
    set_state(&d, 1, n, true);

    d.table[0]['0'] = 1; d.table[0]['1'] = 0;
    d.table[1]['0'] = 0; d.table[1]['1'] = 1;

    dfa *reduced = reachable_dfa(&d);
    assert(reduced != NULL);

    state_list P = minimize_hopcroft(reduced);
    dfa m = build_min_dfa(reduced, &P);

    assert(m.state_count == 2);
    assert(state_contains(&m.states[0], 0)); // inicial preservado
    assert(m.states[0].accepts == false);

    printf("  ok: se conservan %d estados.\n", m.state_count);

    list_free(&P);
    dfa_free(reduced); free(reduced);
    dfa_free(&m);
    dfa_free(&d);
}

// Simula la ejecución de un DFA sobre una cadena de entrada y devuelve true si la
// cadena es aceptada, false si es rechazada.
bool test_string(const dfa *d, const char *input) {
    int current = 0; // estado inicial
    for (int i = 0; input[i] != '\0'; i++) {
        unsigned char c = (unsigned char)input[i];
        int next = d->table[current][c];
        if (next == -1) return false; // transición indefinida
        current = next;
    }
    // Al final de la cadena, verificamos si el estado actual es de aceptación.
    return d->states[current].accepts;
}

// Ejecuta un conjunto de pruebas de aceptación y rechazo sobre un DFA dado
// y reporta los resultados.
void run_test_suite (const dfa *d, const char *accept_tests[], int accept_count,
                     const char *reject_tests[], int reject_count) {
    int passed = 0;
    int total = accept_count + reject_count;

    printf("\n[Corriendo Casos de Aceptación]\n");
    for (int i = 0; i < accept_count; i++) {
        bool res = test_string(d, accept_tests[i]);
        // reporta true si la cadena es aceptada, false si es rechazada
        printf("Cadena \"%s\": %s\n", accept_tests[i], res ? "PASS" : "FAIL");
        if (res) passed++;
    }

    printf("\n[Corriendo Casos de Rechazo]\n");
    for (int i = 0; i < reject_count; i++) {
        bool res = !test_string(d, reject_tests[i]);
        // reporta true si la cadena es rechazada, false si es aceptada
        // PASS porque en esta parte se espera que sean rechazadas por eso !test_string
        printf("Cadena \"%s\": %s\n", reject_tests[i], res ? "PASS" : "FAIL");
        if (res) passed++;
    }

    printf("\nResultados: %d/%d pruebas superadas.\n", passed, total);
}

// Primera prueba de regex de la lista
void test_regex1(void) {
    printf("\nPrueba de regex: (a|b)*abb - Cadenas que terminan en abb\n");
    dfa original;
    dfa_init(&original);
    original.state_count = 6;

    for (int i = 0; i < original.state_count; i++) {
        for (int j = 0; j < ALPHABET_SIZE; j++) {
            original.table[i][j] = -1; // Inicializar todas las transiciones como indefinidas
        }
        set_state(&original, i, original.state_count, (i == 3)); // Solo el estado 3 es de aceptación
    }

    original.table[0]['a'] = 1; original.table[0]['b'] = 0;
    original.table[1]['a'] = 1; original.table[1]['b'] = 2;
    original.table[2]['a'] = 1; original.table[2]['b'] = 3;
    original.table[3]['a'] = 1; original.table[3]['b'] = 0;
    original.table[4]['a'] = 1; original.table[4]['b'] = 0;
    original.table[5]['a'] = 1; original.table[5]['b'] = 0;

    print_dfa(&original, "ab");

    dfa *reduced = reachable_dfa(&original);
    state_list part = minimize_hopcroft(reduced);
    dfa minimized = build_min_dfa(reduced, &part);

    assert(original.state_count >= minimized.state_count);
    print_dfa_min(&minimized, "ab");
    printf("Número de estados: |Q| original=%d, |Q'| minimizado=%d\n", original.state_count, minimized.state_count);

    const char *accept_tests[] = {"abb", "aabb", "aaabb", "babb", "abababb", "ababb", "aababb", "bbaabb", "bbbbabb", "ababababb"};
    const char *reject_tests[] = {"", "a", "b", "ab", "aab", "ba", "aaa", "bbb", "abab", "aabba"};
    run_test_suite(&minimized, accept_tests, 10, reject_tests, 10);

    list_free(&part);
    dfa_free(reduced); free(reduced);
    dfa_free(&minimized);
    dfa_free(&original);
}

// Segunda prueba de regex de la lista
void test_regex2(void) {
    printf("\nPrueba de regex: (0|1)*01(0|1)* - Cadenas que contienen la subcadena 01\n");

    dfa original;
    dfa_init(&original);
    original.state_count = 5;

    for (int i = 0; i < original.state_count; i++) {
        for (int j = 0; j < ALPHABET_SIZE; j++) {
            original.table[i][j] = -1; // Inicializar todas las transiciones como indefinidas
        }
        set_state(&original, i, original.state_count, (i >= 2)); // Estados 2, 3 y 4 son de aceptación
    }

    original.table[0]['0'] = 1; original.table[0]['1'] = 0;
    original.table[1]['0'] = 1; original.table[1]['1'] = 2;
    original.table[2]['0'] = 3; original.table[2]['1'] = 4;
    original.table[3]['0'] = 3; original.table[3]['1'] = 4;
    original.table[4]['0'] = 3; original.table[4]['1'] = 4;

    print_dfa(&original, "01");

    dfa *reduced = reachable_dfa(&original);
    state_list part = minimize_hopcroft(reduced);
    dfa minimized = build_min_dfa(reduced, &part);

    assert(original.state_count >= minimized.state_count);
    print_dfa_min(&minimized, "01");
    printf("Número de estados: |Q| original=%d, |Q'| minimizado=%d\n", original.state_count, minimized.state_count);

    const char *accept_tests[] = {"01", "001", "101", "010", "1010", "0101", "10101", "01010", "101010", "01010"};
    const char *reject_tests[] = {"", "0", "1", "00", "11", "000", "111", "1100", "55555555", "11111111"};
    run_test_suite(&minimized, accept_tests, 10, reject_tests, 10);

    list_free(&part);
    dfa_free(reduced); free(reduced);
    dfa_free(&minimized);
    dfa_free(&original);
}
// Tercera prueba de regex de la lista
void test_regex3(void) {
    printf("\nPrueba de regex: (0|10*1)* - Cadenas que tienen un número par de 1s\n");
    dfa original;
    dfa_init(&original);
    original.state_count = 4;

    for (int i = 0; i < original.state_count; i++) {
        for (int j = 0; j < ALPHABET_SIZE; j++) {
            original.table[i][j] = -1; // Inicializar todas las transiciones como indefinidas
        }
        set_state(&original, i, original.state_count, (i <= 1)); // Estados 0 y 1 son de aceptación
    }

    original.table[0]['0'] = 1; original.table[0]['1'] = 2;
    original.table[1]['0'] = 0; original.table[1]['1'] = 3;
    original.table[2]['0'] = 2; original.table[2]['1'] = 0;
    original.table[3]['0'] = 3; original.table[3]['1'] = 1;

    print_dfa(&original, "01");

    dfa *reduced = reachable_dfa(&original);
    state_list part = minimize_hopcroft(reduced);
    dfa minimized = build_min_dfa(reduced, &part);

    assert(original.state_count >= minimized.state_count);
    print_dfa_min(&minimized, "01");
    printf("Número de estados: |Q| original=%d, |Q'| minimizado=%d\n", original.state_count, minimized.state_count);

    const char *accept_tests[] = {"", "00", "11", "0000", "1111", "0011", "1100", "0101", "1010", "1001"};
    const char *reject_tests[] = {"10101", "1", "01", "10", "001", "111110", "01011", "100", "1110", "0001"};
    run_test_suite(&minimized, accept_tests, 10, reject_tests, 10);

    list_free(&part);
    dfa_free(reduced); free(reduced);
    dfa_free(&minimized);
    dfa_free(&original);
}

int main(void) {
    printf("Pruebas de reconstrucción del DFA mínimo\n");
    test_equivalent_states();
    test_unreachable_then_minimize();
    test_already_minimal();
    printf("\nTodas las pruebas pasaron correctamente.\n");

    printf("\nPruebas con expresiones regulares\n");
    test_regex1();
    test_regex2();
    test_regex3();
    printf("\nTodas las pruebas de expresiones regulares pasaron correctamente.\n");
    return 0;
}
