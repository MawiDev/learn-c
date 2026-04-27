# Lezione 10 (medi) — Gestione degli errori

Il C non ha eccezioni. Gli errori si comunicano con **valori di ritorno** (e a volte `errno`). Saper gestire gli errori è la differenza tra un programma che funziona "a volte" e uno robusto.

## Il contratto classico

Funzioni che possono fallire ritornano un **codice di errore** e, se serve, scrivono il risultato tramite un puntatore di output.

```c
/* Ritorna 0 in caso di successo, -1 in errore (errno impostato). */
int leggi_intero(const char *path, int *out);
```

Oppure convenzioni come:
- `NULL` in caso di errore per chi ritorna puntatori (es. `fopen`, `malloc`).
- `-1` / negativo per chi ritorna `int`.
- `EOF` per I/O.

L'importante è la **coerenza** all'interno di un progetto.

## `errno`

`<errno.h>` definisce `errno`, una variabile globale (per-thread nei sistemi moderni) che molte funzioni di sistema settano quando falliscono.

```c
#include <errno.h>
#include <string.h>

FILE *f = fopen(path, "r");
if (f == NULL) {
    fprintf(stderr, "fopen(%s): %s\n", path, strerror(errno));
    return -1;
}
```

- `strerror(errno)` → stringa descrittiva.
- `perror("prefisso")` → stampa `prefisso: descrizione\n` su `stderr`.

Pattern: controlla il valore di ritorno **prima** di leggere `errno`. Se il valore di ritorno dice "OK", `errno` può essere spazzatura.

## Assertions (`<assert.h>`)

```c
#include <assert.h>

void dividi(int n, int d) {
    assert(d != 0);     // in release (NDEBUG) sparisce
    ...
}
```

`assert` serve a verificare **invarianti di programma**: cose che, se false, indicano un **bug**. Non usarlo per validare input dell'utente — usa il check esplicito.

Compila con `-DNDEBUG` per disabilitare tutti gli `assert` in release (è lo standard).

Per asserzioni che non vuoi mai perdere, usa un macro tua:
```c
#define ENFORCE(cond) do { if (!(cond)) abort(); } while (0)
```

## Il pattern "single cleanup"

Per funzioni con più acquisizioni di risorse, il `goto` per un cleanup unico è idiomatico:

```c
int fai_cose(const char *path) {
    int rc = -1;
    FILE *f = NULL;
    char *buf = NULL;

    f = fopen(path, "r");
    if (!f) goto out;

    buf = malloc(4096);
    if (!buf) goto out;

    /* ... lavoro ... */

    rc = 0;    /* successo */

out:
    free(buf);
    if (f) fclose(f);
    return rc;
}
```

Questo pattern (usato massicciamente nel kernel Linux) è la versione C di try/finally. Ogni fallimento salta a `out`, il cleanup gestisce "null-safely" tutto quello che è stato allocato finora.

## Propagazione degli errori

```c
int caricamento(...) {
    int rc = apri_file();
    if (rc < 0) return rc;

    rc = leggi_header();
    if (rc < 0) return rc;

    rc = valida();
    if (rc < 0) return rc;

    return 0;
}
```

Non nascondere gli errori. Non ignorarli. Passali al chiamante finché non c'è un livello adatto a decidere cosa fare.

## Idioma `_Result` (moderno)

Alcuni progetti preferiscono strutture "result":

```c
typedef enum { OK, ERR_IO, ERR_FORMAT, ERR_OOM } Status;

typedef struct {
    Status  status;
    int     valore;
} IntResult;

IntResult parse_int(const char *s);
```

Più ingombrante, ma tutto è tipizzato e autocompletato. Utile per interfacce pubbliche.

## Quando `abort`/`exit`

- `exit(EXIT_FAILURE)` → termina ordinatamente (flusha stdio, chiama gli `atexit`).
- `abort()` → termina subito (SIGABRT), core dump. Usa quando hai rilevato uno **stato impossibile**.

Non usare `exit()` nelle **librerie**: una libreria non deve decidere la vita del programma chiamante. Ritorna un errore e lascia decidere.

## `setjmp`/`longjmp` (non userai)

Un salto non-locale stile "eccezioni povere". Pericoloso: non chiama distruttori (in C non ce ne sono), non interagisce bene con risorse dinamiche. Evita, a meno che tu non stia scrivendo un runtime.

## Logging strutturato

Per progetti seri, definisci un layer di logging:

```c
typedef enum { LOG_DEBUG, LOG_INFO, LOG_WARN, LOG_ERROR } LogLevel;

void log_msg(LogLevel lvl, const char *fmt, ...);
```

Poi nel codice:
```c
log_msg(LOG_WARN, "valore inatteso: %d", x);
```

Ti consente di:
- attivare/disattivare livelli a runtime,
- aggiungere timestamp, thread id, eccetera,
- direzionare su file o syslog.

## Controllo di ritorni importanti

Non ignorare il valore di ritorno di:
- `malloc`, `calloc`, `realloc` → NULL significa OOM.
- `fopen`, `fclose`, `fread`, `fwrite`.
- `scanf`/`fscanf` → numero di campi letti.
- `snprintf` → troncamento.

GCC/Clang aiutano con `-Wall` e, per funzioni tue, con `[[nodiscard]]` (C23) o `__attribute__((warn_unused_result))`.

## Esercizi

1. Riscrivi `fai_cose` (vista sopra) aprendo 2 file e un buffer, con cleanup single-exit via `goto`.
2. Scrivi una funzione `parse_int_stretto(const char *s, int *out)` che ritorna 0 su successo, -1 altrimenti, **senza** accettare spazi o caratteri extra.
3. Scrivi un mini-logger con livelli e filtro configurabile.
4. Inietta volutamente un errore di allocazione (simula `malloc` che ritorna NULL) e verifica che il cleanup lasci il sistema in uno stato pulito.
5. Compila con `-DNDEBUG` e senza, e osserva come gli `assert` spariscono.
