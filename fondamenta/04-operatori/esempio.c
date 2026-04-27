/*
 * Lezione 04 — Operatori.
 */

#include <stdio.h>
#include <stdint.h>

int main(void) {
    /* Aritmetici */
    int a = 17, b = 5;
    printf("%d + %d = %d\n", a, b, a + b);
    printf("%d - %d = %d\n", a, b, a - b);
    printf("%d * %d = %d\n", a, b, a * b);
    printf("%d / %d = %d (int)\n", a, b, a / b);
    printf("%d %% %d = %d\n", a, b, a % b);

    /* Ternario */
    int max = (a > b) ? a : b;
    printf("max = %d\n", max);

    /* Bitwise: manipolazione di flag */
    uint8_t permessi = 0;
    const uint8_t READ  = 1u << 0;   /* 0b001 */
    const uint8_t WRITE = 1u << 1;   /* 0b010 */
    const uint8_t EXEC  = 1u << 2;   /* 0b100 */

    permessi |= READ;                /* set */
    permessi |= EXEC;
    printf("permessi  = 0b%03o\n", permessi);
    printf("leggibile = %s\n", (permessi & READ)  ? "si" : "no");
    printf("scrivibile= %s\n", (permessi & WRITE) ? "si" : "no");

    permessi &= ~EXEC;               /* clear */
    printf("dopo clear EXEC: 0b%03o\n", permessi);

    permessi ^= WRITE;               /* toggle */
    printf("dopo toggle WRITE: 0b%03o\n", permessi);

    /* Short-circuit: divisione sicura */
    int div = 0;
    if (b != 0 && (a / b) > 2) {
        printf("a/b > 2\n");
    }
    (void)div;

    return 0;
}
