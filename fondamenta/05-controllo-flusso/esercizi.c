/*
 * Lezione 05 — Esercizi: controllo di flusso.
 *
 * Compila con:
 *   gcc -std=c17 -Wall -Wextra -Wpedantic esercizi.c -o esercizi
 */

#include <stdio.h>

/* ============================================================
 * Esercizio 1 — Voto in scala letterale (con if/else if).
 * ============================================================
 *
 * Dato un intero voto da 0 a 30:
 *   30          -> "ottimo"
 *   24..29      -> "buono"
 *   18..23      -> "sufficiente"
 *   < 18        -> "insufficiente"
 *   fuori 0..30 -> "voto non valido"
 *
 * Ritorna una stringa C (const char*).
 */
static const char *valuta_voto_if(int voto) {
    (void)voto;
    /* TODO: implementa con if/else if/else. */
    return "TODO";
}

static void esercizio_1(void) {
    printf("== Esercizio 1 — voto con if ==\n");
    int voti[] = {30, 28, 24, 22, 18, 17, -1, 35};
    for (size_t i = 0; i < sizeof voti / sizeof voti[0]; ++i) {
        printf("%2d -> %s\n", voti[i], valuta_voto_if(voti[i]));
    }
}

/* ============================================================
 * Esercizio 2 — Stesso voto con switch.
 * ============================================================
 *
 * switch funziona solo con interi e valori costanti, quindi non puoi fare
 * "case 24..29". Trucco: dividi il voto per una soglia e fai switch sul
 * risultato. Per esempio voto/6 mappa 30 -> 5, 24..29 -> 4, 18..23 -> 3.
 *
 * Sfrutta anche il fallthrough (case senza break) per raggruppare casi.
 */
static const char *valuta_voto_switch(int voto) {
    (void)voto;
    /* TODO: implementa con switch. */
    return "TODO";
}

static void esercizio_2(void) {
    printf("\n== Esercizio 2 — voto con switch ==\n");
    int voti[] = {30, 28, 24, 22, 18, 17, -1, 35};
    for (size_t i = 0; i < sizeof voti / sizeof voti[0]; ++i) {
        printf("%2d -> %s\n", voti[i], valuta_voto_switch(voti[i]));
    }
}

/* ============================================================
 * Esercizio 3 — Macchina a stati IDLE -> RUN -> STOP.
 * ============================================================
 *
 * Definisci un enum con i tre stati. Leggi caratteri da stdin in un loop:
 *   'r' -> passa a RUN  (solo se eri in IDLE)
 *   's' -> passa a STOP (solo se eri in RUN)
 *   'q' -> esci dal loop
 * Stampa lo stato dopo ogni transizione, e segnala le transizioni invalide.
 *
 * Hint: usa un while(scanf(" %c", &c) == 1) e uno switch sulla coppia
 * (stato, comando) — o piu' semplicemente switch dentro switch.
 */
typedef enum { STATO_IDLE, STATO_RUN, STATO_STOP } Stato;

static const char *nome_stato(Stato s) {
    switch (s) {
        case STATO_IDLE: return "IDLE";
        case STATO_RUN:  return "RUN";
        case STATO_STOP: return "STOP";
    }
    return "?";
}

static void esercizio_3(void) {
    printf("\n== Esercizio 3 — macchina a stati (digita r, s, q) ==\n");
    Stato s = STATO_IDLE;
    printf("stato iniziale: %s\n", nome_stato(s));
    /* TODO: leggi un char alla volta e gestisci le transizioni come da spec.
     *       Fermati quando l'utente digita 'q'. */
}

/* ============================================================
 * Esercizio 4 — FizzBuzz (1..100).
 * ============================================================
 *
 *   divisibile per 15 -> "FizzBuzz"
 *   divisibile per 3  -> "Fizz"
 *   divisibile per 5  -> "Buzz"
 *   altrimenti        -> il numero
 *
 * Attenzione all'ordine: testa prima la condizione "per 15".
 */
static void esercizio_4(void) {
    printf("\n== Esercizio 4 — FizzBuzz 1..100 ==\n");
    /* TODO: stampa la sequenza FizzBuzz da 1 a 100. */
}

int main(void) {
    esercizio_1();
    esercizio_2();
    esercizio_3();
    esercizio_4();
    return 0;
}
