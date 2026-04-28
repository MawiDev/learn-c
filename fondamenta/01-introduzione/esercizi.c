/*
 * Lezione 01 — Esercizi: introduzione e compilazione.
 *
 * Compila con:
 *   gcc -std=c17 -Wall -Wextra -Wpedantic esercizi.c -o esercizi
 * Esegui:
 *   ./esercizi
 *
 * Gli esercizi di questa prima lezione sono soprattutto da fare in shell:
 * leggi le istruzioni di ognuno e segui i passi. Il file C qui sotto e' la
 * "palestra" dove modificare il codice per gli esercizi 2 e 3.
 */

#include <stdio.h>

/* ============================================================
 * Esercizio 1 — Compila ed esegui questo file.
 * ============================================================
 *
 *   gcc -std=c17 -Wall -Wextra -Wpedantic esercizi.c -o esercizi
 *   ./esercizi
 *
 * Verifica di vedere il messaggio "Esercizi pronti.". Se sbagli un flag,
 * leggi attentamente il messaggio del compilatore: e' la tua prima palestra
 * di "interpretare gli errori".
 */

/* ============================================================
 * Esercizio 2 — Cambia il messaggio e ricompila.
 * ============================================================
 *
 * Modifica la printf qui sotto in modo che stampi il TUO nome. Ricompila
 * ed esegui. Prova ad aggiungere una seconda printf con il giorno di oggi.
 */

/* ============================================================
 * Esercizio 3 — Variabile non inizializzata + -Werror.
 * ============================================================
 *
 * Decommenta il blocco marcato TODO-3 qui sotto e ricompila con:
 *
 *   gcc -std=c17 -Wall -Wextra -Wpedantic -Werror esercizi.c -o esercizi
 *
 * Cosa succede? Leggi il messaggio del compilatore. Poi inizializza la
 * variabile a 0 e ricompila: ora deve passare. Questo e' "il compilatore
 * che ti difende": senza -Werror il warning passerebbe inosservato.
 */

/* ============================================================
 * Esercizio 4 — Cosa fa il preprocessore.
 * ============================================================
 *
 *   gcc -E esercizi.c | less
 *
 * Esegui in shell. Vedrai che il file esploda in centinaia di righe:
 * <stdio.h> e tutto cio' che include vengono "incollati" qui prima della
 * compilazione vera. Questa e' la prima fase del ciclo di compilazione.
 */

int main(void) {
    /* TODO-2: cambia il messaggio con il tuo nome. */
    printf("Esercizi pronti.\n");
    printf("Riccardo\n");
    printf("28-04-2026\n");


    /* TODO-3: rimuovi questo #if 0 / #endif per attivare il blocco e
     * scopri perche' il compilatore con -Werror ti ferma. */
    int x=0;
    printf("x vale: %d\n", x);

    return 0;
}
