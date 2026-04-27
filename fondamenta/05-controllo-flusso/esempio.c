/*
 * Lezione 05 — Controllo di flusso.
 */

#include <stdio.h>

typedef enum { STATO_IDLE, STATO_RUN, STATO_STOP } Stato;

static const char *nome_stato(Stato s) {
    switch (s) {
        case STATO_IDLE: return "IDLE";
        case STATO_RUN:  return "RUN";
        case STATO_STOP: return "STOP";
    }
    return "?";
}

int main(void) {
    /* if / else if / else */
    int voto = 27;
    if (voto >= 28) {
        printf("ottimo\n");
    } else if (voto >= 24) {
        printf("buono\n");
    } else if (voto >= 18) {
        printf("sufficiente\n");
    } else {
        printf("insufficiente\n");
    }

    /* switch con enum */
    Stato s = STATO_RUN;
    switch (s) {
        case STATO_IDLE:
            printf("in attesa...\n");
            break;
        case STATO_RUN:
            printf("in esecuzione\n");
            break;
        case STATO_STOP:
            printf("fermo\n");
            break;
    }
    printf("stato: %s\n", nome_stato(s));

    /* FizzBuzz fino a 15 */
    printf("\nFizzBuzz 1..15\n");
    for (int i = 1; i <= 15; ++i) {
        if (i % 15 == 0)      printf("FizzBuzz\n");
        else if (i % 3 == 0)  printf("Fizz\n");
        else if (i % 5 == 0)  printf("Buzz\n");
        else                  printf("%d\n", i);
    }

    return 0;
}
