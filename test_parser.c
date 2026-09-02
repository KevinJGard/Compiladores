

#include <stdio.h>
#include "regex.h"

static void probar(const char *entrada) {
    regex r = parse_regex(entrada);

    printf("%-12s -> ", entrada);

    if (r.error != REGEX_OK) {
        printf("ERROR (codigo %d)\n", r.error);
        return;
    }

    for (int i = 0; i < r.size; i++)
        printf("%c", r.items[i].value);
    printf("\n");

    regex_free(&r);
}

int main(void) {
    probar("abc");
    probar("ab*c");
    probar("a(b|c)*");
    probar("a+b?");
    probar("(ab)*");
    probar("a|b");
    return 0;
}