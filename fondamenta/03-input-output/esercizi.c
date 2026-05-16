/*
 * Lezione 03 — Esercizi: input e output.
 *
 * Compila con:
 *   gcc -std=c17 -Wall -Wextra -Wpedantic esercizi.c -o esercizi
 *
 * Esegui:
 *   ./esercizi
 *
 * Per testare la separazione stdout/stderr (esercizio 4):
 *   ./esercizi > out.txt           # stderr resta sul terminale
 *   ./esercizi 2> err.txt          # stdout resta sul terminale
 */

#include <stdio.h>
#include <string.h>

/* ============================================================
 * Esercizio 1 — Saluto personalizzato.
 * ============================================================
 *
 * Chiedi all'utente nome ed eta'. Stampa: "Ciao <nome>, hai <eta> anni!".
 *
 * Suggerimenti:
 *   - usa fgets per il nome (mai %s senza limite).
 *   - togli il '\n' finale che fgets lascia.
 *   - usa scanf("%d", &eta) e CONTROLLA il valore di ritorno.
 */
static void esercizio_1(void) {
    printf("== Esercizio 1 — saluto ==\n");
    /* TODO: leggi nome con fgets, eta con scanf, stampa il saluto. */
    char nome[64];
    int eta;
    printf("Inserisci nome: ");
    if(fgets(nome,sizeof nome,stdin)== NULL) {
        fprintf(stderr,"errore nome\n");
    }
    size_t len = strlen(nome);
    if (len > 0 && nome[len-1] == '\n') nome[len-1] = '\0';
    printf("Inserisci eta: ");
    if(scanf("%d",&eta) != 1) {
        fprintf(stderr,"errore eta\n");
    }
    printf ("Ciao %s, hai %d anni\n", nome, eta);
}

/* ============================================================
 * Esercizio 2 — Quattro operazioni su due numeri.
 * ============================================================
 *
 * Leggi due numeri (decidi tu se int o double) e stampa:
 *   somma, differenza, prodotto, quoziente.
 *
 * Attenzione: se usi int, il quoziente e' una divisione intera. Se vuoi
 * il risultato decimale del quoziente, usa double oppure cast esplicito.
 * Gestisci la divisione per zero!
 */
static void esercizio_2(void) {
    printf("\n== Esercizio 2 — 4 operazioni ==\n");
    int n1, n2;

    printf("n1: ");
    if (scanf("%d", &n1) != 1) {
        fprintf(stderr, "n1 non valido\n");
        return;
    }
    printf("n2: ");
    if (scanf("%d", &n2) != 1) {
        fprintf(stderr, "n2 non valido\n");
        return;
    }

    printf("somma: %d\n", n1 + n2);
    printf("diff : %d\n", n1 - n2);
    printf("prod : %d\n", n1 * n2);

    if (n2 == 0) {
        fprintf(stderr, "quoz : divisione per zero\n");
    } else {
        printf("quoz : %.3f\n", (double)n1 / n2);
    }
}

/* ============================================================
 * Esercizio 3 — Inverti una riga letta da fgets.
 * ============================================================
 *
 * Leggi una riga con fgets in un buffer di 128 byte. Togli il '\n' finale.
 * Stampa la riga al contrario, carattere per carattere.
 *
 * Hint: una volta calcolata strlen, scorri da len-1 fino a 0 con putchar.
 */
static void esercizio_3(void) {
    printf("\n== Esercizio 3 — riga al contrario ==\n");
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
    printf("Scrivi una riga: ");
    char riga[128];
    if (fgets(riga, sizeof riga, stdin) == NULL) {
        fprintf(stderr, "lettura fallita\n");
        return;
    }
    /* Toglie il '\n' finale se presente. */
    size_t len = strlen(riga);
    if (len > 0 && riga[len - 1] == '\n') {
        riga[--len] = '\0';
    }
    for (size_t i = 0; i < len; i++) {
        putchar(riga[len - 1 - i]);
    }
    putchar('\n');
}

/* ============================================================
 * Esercizio 4 — stdout vs stderr.
 * ============================================================
 *
 * Stampa un messaggio "normale" su stdout e un messaggio "di errore" su
 * stderr (usa fprintf(stderr, ...)). Esegui poi:
 *   ./esercizi > out.txt
 *   ./esercizi 2> err.txt
 * e osserva quale dei due messaggi finisce nel file e quale a schermo.
 */
static void esercizio_4(void) {
    printf("\n== Esercizio 4 — stdout / stderr ==\n");
    /* TODO: stampa due messaggi, uno su stdout, uno su stderr. */
    fprintf(stdout,"ciao\n");
    fprintf(stderr, "errore\n");
}

int main(void) {
    esercizio_1();
    esercizio_2();
    esercizio_3();
    esercizio_4();
    return 0;
}
