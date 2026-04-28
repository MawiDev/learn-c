/*
 * Lezione 09 — Esercizi: stringhe.
 *
 * Compila con:
 *   gcc -std=c17 -Wall -Wextra -Wpedantic esercizi.c -o esercizi
 */

#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <ctype.h>

/* ============================================================
 * Esercizio 1 — mia_strlen.
 * ============================================================
 *
 * Implementa strlen senza usare <string.h>. Una stringa C finisce al primo
 * '\0'. Usa un puntatore che avanza finche' *p != '\0'.
 */
static size_t mia_strlen(const char *s) {
    (void)s;
    /* TODO: scorri fino a '\0' e ritorna la differenza di puntatori. */
    return 0;
}

static void esercizio_1(void) {
    printf("== Esercizio 1 — mia_strlen ==\n");
    printf("'%s' -> %zu (atteso 5)\n", "Ciao!", mia_strlen("Ciao!"));
    printf("'%s' -> %zu (atteso 0)\n", "",      mia_strlen(""));
}

/* ============================================================
 * Esercizio 2 — a_maiuscolo in-place.
 * ============================================================
 *
 * Modifica la stringa convertendo ogni lettera in maiuscolo.
 * ATTENZIONE: <ctype.h> richiede unsigned char (o EOF), altrimenti UB su
 * char con valori > 127. Usa il cast esplicito (unsigned char).
 *
 * Importante: se passi una stringa LETTERALE ("ciao") sei in memoria
 * read-only e crashi. Per testare usa un array char buf[] = "ciao".
 */
static void a_maiuscolo(char *s) {
    (void)s;
    /* TODO: scorri il puntatore e applica toupper((unsigned char)*s). */
}

static void esercizio_2(void) {
    printf("\n== Esercizio 2 — a_maiuscolo ==\n");
    char buf[] = "ciao mondo";
    a_maiuscolo(buf);
    printf("'%s' (atteso 'CIAO MONDO')\n", buf);
}

/* ============================================================
 * Esercizio 3 — Conta vocali.
 * ============================================================
 *
 * Ritorna il numero di vocali (a, e, i, o, u, sia maiuscole che minuscole)
 * in una stringa.
 */
static size_t conta_vocali(const char *s) {
    (void)s;
    /* TODO: scorri la stringa e conta le vocali. */
    return 0;
}

static void esercizio_3(void) {
    printf("\n== Esercizio 3 — conta_vocali ==\n");
    printf("'Ciao Mondo' -> %zu (atteso 4)\n", conta_vocali("Ciao Mondo"));
    printf("'BCDFG'      -> %zu (atteso 0)\n", conta_vocali("BCDFG"));
}

/* ============================================================
 * Esercizio 4 — palindroma (case-insensitive).
 * ============================================================
 *
 * Una stringa e' palindroma se letta da sinistra a destra e' uguale a
 * letta da destra a sinistra. Confronta ignorando maiuscole/minuscole.
 *
 * Hint: due indici i (da 0) e j (da len-1), confronta tolower e avanza
 * finche' i < j.
 */
static bool palindroma(const char *s) {
    (void)s;
    /* TODO: implementa il confronto con due indici. */
    return false;
}

static void esercizio_4(void) {
    printf("\n== Esercizio 4 — palindroma ==\n");
    const char *casi[] = {"anna", "Anna", "ciao", "Otto", "abba", "abc"};
    for (size_t i = 0; i < sizeof casi / sizeof casi[0]; ++i) {
        printf("'%s' -> %s\n", casi[i], palindroma(casi[i]) ? "si'" : "no");
    }
}

/* ============================================================
 * Esercizio 5 — snprintf con gestione del troncamento.
 * ============================================================
 *
 * Usa snprintf per costruire una stringa nel formato "ID=%04d, nome=%s"
 * dentro un buffer di 64 byte. snprintf:
 *   - non fa MAI overflow (e' la sua promessa);
 *   - ritorna il numero di byte CHE AVREBBE scritto (escluso '\0');
 *   - se il valore di ritorno e' >= dimensione del buffer, c'e' stato
 *     troncamento — segnalalo.
 *
 * Testa due volte: una con un nome corto, una con un nome lunghissimo che
 * forzi il troncamento. Verifica che il programma non crashi mai.
 */
static void formatta_id(char *buf, size_t bufcap, int id, const char *nome) {
    (void)buf; (void)bufcap; (void)id; (void)nome;
    /* TODO: usa snprintf, controlla il valore di ritorno, stampa se c'e'
     *       stato troncamento. */
}

static void esercizio_5(void) {
    printf("\n== Esercizio 5 — snprintf sicuro ==\n");
    char buf[64];
    formatta_id(buf, sizeof buf, 7, "Riccardo");
    printf("buf = '%s'\n", buf);

    formatta_id(buf, sizeof buf, 9999,
                "un-nome-volutamente-troppo-lungo-per-stare-in-64-byte-di-buffer");
    printf("buf = '%s'\n", buf);
}

int main(void) {
    esercizio_1();
    esercizio_2();
    esercizio_3();
    esercizio_4();
    esercizio_5();
    return 0;
}
