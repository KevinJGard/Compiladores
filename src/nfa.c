#include <stdlib.h>
#include "nfa.h"

/* Libera la memoria reservada por regex_to_nfa. */
void nfa_free(nfa *n) {
    free(n->transitions);
    n->transitions = NULL;
    n->trans_count = 0;
    n->trans_capacity = 0;
}

transition* question_trans(nfa a) {
  int size_new = a.trans_count + 3;
  transition* arr_trans = malloc(size_new * sizeof(transition));
  if (!arr_trans) {
    return NULL;
  }
  int count = 0;
  int offset = 1;
  transition starts_trans = (transition){0, a.start + offset, EPSILON};
  transition s_a_trans = (transition){0, a.state_count + 1, EPSILON};
  arr_trans[count++] = starts_trans;
  arr_trans[count++] = s_a_trans;
  for (int i = 0; i < a.trans_count; i++) {
    transition trans = a.transitions[i];
    arr_trans[count++] = (transition){trans.from + offset, trans.to + offset, trans.symbol};
  }
  transition accepts = (transition){a.accept + offset, a.state_count + offset, EPSILON};
  arr_trans[count++] = accepts;
  return arr_trans;
}

transition* plus_trans(nfa a) {
  int size_new = a.trans_count + 3;
  transition* arr_trans = malloc(size_new * sizeof(transition));
  if (!arr_trans) {
    return NULL;
  }
  int count = 0;
  int offset = 1;
  transition starts_trans = (transition){0, a.start + offset, EPSILON};
  arr_trans[count++] = starts_trans;
  for (int i = 0; i < a.trans_count; i++) {
    transition trans = a.transitions[i];
    arr_trans[count++] = (transition){trans.from + offset, trans.to + offset, trans.symbol};
  }
  transition internal_trans = (transition){a.accept + offset, a.start + offset, EPSILON};
  transition accepts = (transition){a.accept + offset, a.state_count + offset, EPSILON};
  arr_trans[count++] = internal_trans;
  arr_trans[count++] = accepts;
  return arr_trans;
}

transition* kleene_trans(nfa a) {
  int size_new = a.trans_count + 4;
  transition* arr_trans = malloc(size_new * sizeof(transition));
  if (!arr_trans) {
    return NULL;
  }
  int count = 0;
  int offset = 1;
  transition starts_trans = (transition){0, a.start + offset, EPSILON};
  transition s_a_trans = (transition){0, a.state_count + offset, EPSILON};
  arr_trans[count++] = starts_trans;
  arr_trans[count++] = s_a_trans;
  for (int i = 0; i < a.trans_count; i++) {
    transition trans = a.transitions[i];
    arr_trans[count++] = (transition){trans.from + offset, trans.to + offset, trans.symbol};
  }
  transition internal_trans = (transition){a.accept + offset, a.start + offset, EPSILON};
  transition accepts = (transition){a.accept + offset, a.state_count + offset, EPSILON};
  arr_trans[count++] = internal_trans;
  arr_trans[count++] = accepts;
  return arr_trans;
}

transition* join_trans(nfa a, nfa b) {
  int size_new = a.trans_count + b.trans_count + 4;
  transition* arr_trans = malloc(size_new * sizeof(transition));
  if (!arr_trans) {
    return NULL;
  }
  int count = 0;

  // Offsets de ESTADOS
  int offset_a = 1;
  int offset_b = a.state_count + 1;
  transition init_trans1 = (transition){0, a.start + offset_a, EPSILON};
  transition init_trans2 = (transition){0, b.start + offset_b, EPSILON};
  arr_trans[count++] = init_trans1;
  arr_trans[count++] = init_trans2;
  for (; count < a.trans_count + 2; count++) {
    transition tmp = a.transitions[count - 2];
    arr_trans[count] = (transition){tmp.from + offset_a, tmp.to + offset_a, tmp.symbol};
  }
  int cur = count;
  for (; count < cur + b.trans_count; count++) {
    transition tmp = b.transitions[count - cur];
    arr_trans[count] = (transition){tmp.from + offset_b, tmp.to + offset_b, tmp.symbol};
  }
  transition acc_trans1 = (transition){a.accept + offset_a, a.state_count + b.state_count + 1, EPSILON};
  transition acc_trans2 = (transition){b.accept + offset_b, a.state_count + b.state_count + 1, EPSILON};
  arr_trans[count++] = acc_trans1;
  arr_trans[count++] = acc_trans2;
  return arr_trans;
}

transition* concat_trans(nfa a, nfa b) {
  int size_new = a.trans_count + b.trans_count + 1;
  transition* arr_trans = malloc(size_new * sizeof(transition));
  if (!arr_trans) {
    return NULL;
  }
  int count = 0;
  for (; count < a.trans_count; count++) {
    arr_trans[count] = a.transitions[count];
  }
  transition e_trans = (transition){a.accept, b.start + a.state_count, EPSILON};
  arr_trans[count++] = e_trans;
  for (; count < size_new; count++) {
    arr_trans[count] = (transition){a.state_count + b.transitions[count - a.trans_count - 1].from, a.state_count + b.transitions[count - a.trans_count - 1].to, b.transitions[count - a.trans_count - 1].symbol};
  }
  return arr_trans;
}

nfa build_simple_nfa(regex_item item) {
  nfa simple = {0};
  simple.transitions = malloc(sizeof(transition));
  if (!simple.transitions) {
    simple.error = 1;
    return simple;
  }
  simple.trans_count = 1;
  simple.trans_capacity = 1;
  simple.state_count = 2;
  simple.start = 0;
  simple.accept = 1;
  transition t1 = (transition){0, 1, item.value};
  simple.transitions[0] = t1;
  return simple;
}

// Se concatena b después de a
nfa concat(nfa a, nfa b) {
  nfa new_aut = {0};
  int size_new = a.trans_count + b.trans_count + 1;
  new_aut.trans_count = size_new;
  new_aut.trans_capacity = size_new;
  new_aut.state_count = b.state_count + a.state_count;
  new_aut.start = a.start;
  new_aut.accept = b.accept + a.state_count;
  new_aut.transitions = concat_trans(a, b);
  if (!new_aut.transitions) {
    new_aut.error = 1;
    nfa_free(&a);
    nfa_free(&b);
    return new_aut;
  }
  nfa_free(&a);
  nfa_free(&b);
  return new_aut;
}

nfa join(nfa a, nfa b) {
  nfa new_aut = {0};
  int size_new = a.trans_count + b.trans_count + 4;
    new_aut.trans_count = size_new;
    new_aut.trans_capacity = size_new;
    new_aut.state_count = b.state_count + a.state_count + 2; // Agregamos un estado final y uno inicial
    new_aut.start = 0;
    new_aut.accept = new_aut.state_count - 1;
    new_aut.transitions = join_trans(a, b);
    if (!new_aut.transitions) {
      new_aut.error = 1;
      nfa_free(&a);
      nfa_free(&b);
      return new_aut;
    }
    nfa_free(&a);
    nfa_free(&b);
    return new_aut;
}

nfa kleene(nfa a) {
  nfa new_aut = {0};
  int size_new = a.trans_count + 4;
  new_aut.trans_count = size_new;
  new_aut.trans_capacity = size_new;
  new_aut.state_count = a.state_count + 2;
  new_aut.start = 0;
  new_aut.accept = new_aut.state_count - 1;
  new_aut.transitions = kleene_trans(a);
  if (!new_aut.transitions) {
    new_aut.error = 1;
    nfa_free(&a);
    return new_aut;
  }
  nfa_free(&a);
  return new_aut;
}

nfa kleene_plus(nfa a) {
  nfa new_aut = {0};
  int size_new = a.trans_count + 3;
  new_aut.trans_count = size_new;
  new_aut.trans_capacity = size_new;
  new_aut.state_count = a.state_count + 2;
  new_aut.start = 0;
  new_aut.accept = new_aut.state_count - 1;
  new_aut.transitions = plus_trans(a);
  if (!new_aut.transitions) {
    new_aut.error = 1;
    nfa_free(&a);
    return new_aut;
  }
  nfa_free(&a);
  return new_aut;
}

nfa question(nfa a) {
  nfa new_aut = {0};
  int size_new = a.trans_count + 3;
  new_aut.trans_count = size_new;
  new_aut.trans_capacity = size_new;
  new_aut.state_count = a.state_count + 2;
  new_aut.start = 0;
  new_aut.accept = new_aut.state_count - 1;
  new_aut.transitions = question_trans(a);
  if (!new_aut.transitions) {
    new_aut.error = 1;
    nfa_free(&a);
    return new_aut;
  }
  nfa_free(&a);
  return new_aut;
}

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

    size_t rgx_len = (size_t) r.size;
    nfa *stack = malloc(rgx_len * sizeof(nfa));
    if (!stack) {
        return n; 
    }
    int top = 0;
    for (int i = 0; i < r.size; i++) {
      if (r.items[i].type == TOKEN_CHAR) {
	      stack[top++] = build_simple_nfa(r.items[i]);
      }
      else if (r.items[i].type == TOKEN_CONCAT) {
        if (top < 2) goto cleanup_error;
        nfa b = stack[--top];
        nfa a = stack[--top];
        stack[top++] = concat(a, b);
      }
      else if (r.items[i].type == TOKEN_UNION) {
        if (top < 2) goto cleanup_error;
        nfa b = stack[--top];
        nfa a = stack[--top];
        stack[top++] = join(a, b);
      }
      else if (r.items[i].type == TOKEN_STAR) {
        if (top < 1) goto cleanup_error;
        nfa a = stack[--top];
        stack[top++] = kleene(a);
      }
      else if (r.items[i].type == TOKEN_PLUS) {
        if (top < 1) goto cleanup_error;
        nfa a = stack[--top];
        stack[top++] = kleene_plus(a);
      }
      else if (r.items[i].type == TOKEN_QUESTION) {
        if (top < 1) goto cleanup_error;
        nfa a = stack[--top];
        stack[top++] = question(a);
      }
      else {
        goto cleanup_error;
      }
      if (stack[top - 1].error != 0) {
        goto cleanup_error;
      }
    }
    if (top != 1) {
      goto cleanup_error;
    }
    n = stack[0];
    free(stack);
    return n;

cleanup_error:
    n.error = 1;
    free(n.transitions);
    while (top > 0) {
        nfa_free(&stack[--top]);
    }
    free(stack);
    n.transitions = NULL;
    return n;
}
