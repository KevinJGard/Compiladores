#include <stdlib.h>
#include <string.h>
#include "regex.h"

/* 
    - Parser de expresiones regulares

   
     tokenizar
      insertar concatenacion explicita
     Shunting-Yard (conversion a postfijo)
    */

static int puede_ir_izquierda(token_type t) {
    switch (t) {
        case TOKEN_CHAR:
        case TOKEN_RPAREN:
        case TOKEN_STAR:
        case TOKEN_PLUS:
        case TOKEN_QUESTION:
            return 1;
        default:
            return 0;
    }
}


static int puede_ir_derecha(token_type t) {
    switch (t) {
        case TOKEN_CHAR:
        case TOKEN_LPAREN:
            return 1;
        default:
            return 0;
    }
}

/* Precedencia de los operadores */
static int precedencia(token_type t) {
    switch (t) {
        case TOKEN_STAR:
        case TOKEN_PLUS:
        case TOKEN_QUESTION:
            return 3;
        case TOKEN_CONCAT:
            return 2;
        case TOKEN_UNION:
            return 1;
        default:
            return 0;
    }
}

/* Convierte una regex infija en un arreglo de tokens con la
   concatenacion explicita, y la reordena a notacion postfija.

   Revisar siempre r.error antes de usar r.items */
regex parse_regex(const char *regex_str) {
    regex r;
    r.items = NULL;
    r.size  = 0;
    r.error = REGEX_OK;

    size_t n = strlen(regex_str);
    if (n == 0) {
        r.error = REGEX_ERR_EMPTY;
        return r;
    }

    /* Arreglo temporal para la pasada 1 */
    regex_item *tmp = malloc(n * sizeof(regex_item));
    if (tmp == NULL) {
        r.error = REGEX_ERR_MEMORY;
        return r;
    }

    /*  tokenizar */
    int count = 0;
    for (size_t i = 0; i < n; i++) {
        char c = regex_str[i];
        regex_item item;
        item.value = c;

        switch (c) {
            case '|': item.type = TOKEN_UNION;    break;
            case '*': item.type = TOKEN_STAR;     break;
            case '+': item.type = TOKEN_PLUS;     break;
            case '?': item.type = TOKEN_QUESTION; break;
            case '(': item.type = TOKEN_LPAREN;   break;
            case ')': item.type = TOKEN_RPAREN;   break;
            default:  item.type = TOKEN_CHAR;     break;
        }

        tmp[count] = item;
        count++;
    }

    /* Arreglo de salida */
    regex_item *out = malloc(2 * n * sizeof(regex_item));
    if (out == NULL) {
        free(tmp);
        r.error = REGEX_ERR_MEMORY;
        return r;
    }

    /*  insertar concatenaciion*/
    int out_count = 0;
    for (int i = 0; i < count - 1; i++) {
        out[out_count++] = tmp[i];

        if (puede_ir_izquierda(tmp[i].type) &&
            puede_ir_derecha(tmp[i + 1].type)) {
            regex_item concat;
            concat.type  = TOKEN_CONCAT;
            concat.value = '.';
            out[out_count++] = concat;
        }
    }
    out[out_count++] = tmp[count - 1];   /* el ultimo token no tiene par */

    free(tmp);

    /*  Shunting-Yard  */
    regex_item *pila = malloc(n * sizeof(regex_item));
    if (pila == NULL) {
        free(out);
        r.error = REGEX_ERR_MEMORY;
        return r;
    }

    /* En postfijo desaparecen los parentesis, asi que el resultado
       nunca es mas largo que out_count. */
    regex_item *post = malloc(out_count * sizeof(regex_item));
    if (post == NULL) {
        free(out);
        free(pila);
        r.error = REGEX_ERR_MEMORY;
        return r;
    }

    int tope = 0;
    int post_count = 0;

    for (int i = 0; i < out_count; i++) {
        regex_item tok = out[i];

        if (tok.type == TOKEN_CHAR) {
            /* Los simbolos salen de inmediato */
            post[post_count++] = tok;

        } else if (tok.type == TOKEN_LPAREN) {
            /* Barrera: entra a la pila y ahi se queda */
            pila[tope++] = tok;

        } else if (tok.type == TOKEN_RPAREN) {
            /* Vacia la pila hasta encontrar su pareja */
            while (tope > 0 && pila[tope - 1].type != TOKEN_LPAREN) {
                post[post_count++] = pila[--tope];
            }
            if (tope == 0) {              /* nunca aparecio el '(' */
                free(out);
                free(pila);
                free(post);
                r.error = REGEX_ERR_UNBALANCED;
                return r;
            }
            tope--;                       /* tira el '(' sin sacarlo */

        } else {
            /* Operador: saca los de precedencia mayor o igual */
            while (tope > 0 &&
                   pila[tope - 1].type != TOKEN_LPAREN &&
                   precedencia(pila[tope - 1].type) >= precedencia(tok.type)) {
                post[post_count++] = pila[--tope];
            }
            pila[tope++] = tok;
        }
    }

    /* Se acabaron los tokens: vaciar lo que quedo esperando */
    while (tope > 0) {
        if (pila[tope - 1].type == TOKEN_LPAREN) {   /* '(' sin cerrar */
            free(out);
            free(pila);
            free(post);
            r.error = REGEX_ERR_UNBALANCED;
            return r;
        }
        post[post_count++] = pila[--tope];
    }

    free(pila);
    free(out);

    r.items = post;
    r.size  = post_count;
    return r;
}

/* Libera la memoria reservada por parse_regex. */
void regex_free(regex *r) {
    free(r->items);
    r->items = NULL;
    r->size  = 0;
}