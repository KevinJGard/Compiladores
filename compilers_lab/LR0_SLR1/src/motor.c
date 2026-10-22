#include <stdio.h>
#include <string.h>
#include "motor.h"

#define MAX_PILA 8192

/**
 * Imprime el estado actual de la pila, los tokens restantes y la acción tomada.
 */
static void imprimir_traza(const Gramatica *g, const int *estados, Nodo *const *nodos, int tope,
                           const int *tokens, int n, int ip, int paso, int a) {
    printf("%3d | ", paso);
    // Contenido de la pila
    for (int i = 0; i <= tope; i++) {
        if (i > 0) printf("%s ", nodos[i]->etiqueta);
        printf("%d ", estados[i]);
    }
    
    printf("| ");
    // Tokens restantes
    for (int i = ip; i < n; i++) {
        printf("%s ", g->terminales[tokens[i]]);
    }
    printf("$ | ");
    
    // Acción
    if (a == 0) {
        printf("ERROR\n");
    } else if (a == ACEPTAR) {
        printf("aceptar\n");
    } else if (a > 0) {
        printf("shift %d\n", a);
    } else {
        printf("reduce %d (%s)\n", -a, g->reglas[-a - 1].texto);
    }
}

/**
 * Recorre la tabla de acción para el estado actual y obtener los tokens
 * que son válidos en ese estado.
 */
static void llenar_esperados(const Gramatica *g, int estado, char *dst, size_t cap) {
    size_t len = 0;
    dst[0] = '\0';
    
    for (int t = 0; t < NUM_TERMINALES; t++) {
        if (g->accion[estado][t] == 0) { 
            continue;
        }
        const char *nom = g->terminales[t];
        size_t need = strlen(nom) + (len ? 2 : 0);
        
        if (len + need + 1 >= cap) {
            break;
        }
        
        if (len) {
            strcat(dst, ", ");
            len += 2;
        }
        
        strcat(dst, nom);
        len += strlen(nom);
    }
}

/**
 * Ejecuta el ciclo de shift-reduce.
 * 
 */
int motor_analizar(const Gramatica *g, const int *tokens, char lexemas[][LEX_MAX], int n,
                   int traza, Nodo **ast, ErrorSintactico *err) {
    int estados[MAX_PILA];
    Nodo *nodos[MAX_PILA];
    int tope = 0, ip = 0, paso = 0, resultado = 0;

    estados[0] = 0;
    nodos[0] = NULL;
    *ast = NULL;

    for (;;) {
        int s = estados[tope];
        int t = (ip < n) ? tokens[ip] : g->fin_entrada;
        int a = g->accion[s][t];

        if (traza) {
            imprimir_traza(g, estados, nodos, tope, tokens, n, ip, ++paso, a);
        }

        if (a == 0) {                                   /* ERROR */
            err->pos = ip;
            err->estado = s;
            err->token = g->terminales[t];
            llenar_esperados(g, s, err->esperados, sizeof(err->esperados));
            resultado = 0;
            break;
        } 
        if (a == ACEPTAR) {                      /* ACEPTAR */
            *ast = nodos[tope];
            resultado = 1;
            break;
        }
        if (a > 0) {                             /* SHIFT */
            if (tope + 1 >= MAX_PILA) {
                fprintf(stderr, "Pila llena\n");
                resultado = -1;
                break;
            }

            const char *lexema = (ip < n) ? lexemas[ip] : "";
            estados[tope + 1] = a;
            nodos[tope + 1] = nodo_terminal(g->terminales[t], lexema);
            tope++;
            ip++;
        } else {                                        /* REDUCE */
            int k = -a;
            const Regla *regla = &g->reglas[k - 1];
            if (regla->largo > tope) {
                fprintf(stderr, "Reduccion invalida (regla %d)\n", k);
                resultado = -1;
                break;
            }

            Nodo *nuevo = nodo_no_terminal(g->no_terminales[regla->lhs], k, regla->texto,
                                            &nodos[tope - regla->largo + 1], regla->largo);
            tope -= regla->largo;
            int destino = g->ir_a[estados[tope]][regla->lhs];
            if (destino < 0 || tope + 1 >= MAX_PILA) {
                fprintf(stderr, "Goto invalido (estado %d, %s)\n", estados[tope], g->no_terminales[regla->lhs]);
                nodo_liberar(nuevo);
                resultado = -1;
                break;
            }

            estados[tope + 1] = destino;
            nodos[tope + 1] = nuevo;
            tope++;
        }
    }

    if (resultado != 1) {
        for (int i = 1; i <= tope; i++) {
            nodo_liberar(nodos[i]);
        }
    }
    return resultado;
}