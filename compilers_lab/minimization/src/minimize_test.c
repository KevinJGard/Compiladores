#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
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

int main(void) {
    printf("Pruebas de reconstrucción del DFA mínimo\n");
    test_equivalent_states();
    test_unreachable_then_minimize();
    test_already_minimal();
    printf("\nTodas las pruebas pasaron correctamente.\n");
    return 0;
}
