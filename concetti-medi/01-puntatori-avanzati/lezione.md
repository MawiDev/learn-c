# Lezione 01 (medi) — Puntatori avanzati

Hai visto le basi. Ora mettiamo insieme i pezzi.

## Puntatore a puntatore

```c
int  x = 5;
int *p = &x;
int **pp = &p;      // pp contiene l'indirizzo di p

**pp = 42;          // modifica x
```

Utile per:
- modificare un puntatore dall'interno di una funzione,
- array di stringhe in stile `int main(int argc, char **argv)`.

```c
void alloca_intero(int **out) {
    *out = malloc(sizeof(int));
    **out = 0;
}

int *p = NULL;
alloca_intero(&p);
```

## `argv` — l'esempio classico

```c
int main(int argc, char **argv) {
    for (int i = 0; i < argc; ++i) {
        printf("argv[%d] = %s\n", i, argv[i]);
    }
    return 0;
}
```

`argv` è un array di puntatori a stringhe. `argv[0]` è il nome del programma.

## Aritmetica dei puntatori — più a fondo

Tra due puntatori allo stesso array, puoi fare **differenza**: ottieni un `ptrdiff_t` (numero di elementi).

```c
int a[10];
int *start = a;
int *cur   = a + 7;
ptrdiff_t diff = cur - start;   // 7
```

Idioma utile: scandire una stringa e calcolare la lunghezza.

```c
size_t mia_strlen(const char *s) {
    const char *p = s;
    while (*p) ++p;
    return (size_t)(p - s);
}
```

## `restrict` (C99) — una promessa al compilatore

```c
void somma(int *restrict dst, const int *restrict a, const int *restrict b, size_t n) {
    for (size_t i = 0; i < n; ++i) dst[i] = a[i] + b[i];
}
```

`restrict` dice al compilatore: "gli indirizzi di questi puntatori **non si sovrappongono**". Abilita ottimizzazioni (vettorizzazione, loop unroll). Se menti → UB.

Usa `restrict` in funzioni tipo `memcpy`, `strcpy`, routine matematiche. Non a caso `memcpy` nella sua firma C99 dichiara i puntatori come `restrict`.

## Puntatori come "viste" su memoria

Un pattern comune: avere un buffer e più puntatori/cursori sopra di esso.

```c
char buffer[1024];
char *write = buffer;
char *end   = buffer + sizeof buffer;

while (write < end && /* c'è altro da scrivere */) {
    *write++ = ...;
}
```

`*p++` è un'idiomatismo: "scrivi/leggi dove punta, poi incrementa".

## Puntatori a funzioni — anticipazione

Sì, anche le funzioni hanno indirizzi. Vedremo nella lezione dedicata.

## `void *` — conversione sicura

```c
void *blob = ...;
int  *pi   = blob;        // in C: conversione implicita OK
const char *s = blob;
```

Idiomi utili:

```c
memset(buffer, 0, sizeof buffer);
memcpy(dst, src, n);
```

Entrambe lavorano su `void *` senza preoccuparsi del tipo.

## Controlli prima di dereferenziare

Regola d'oro in codice robusto:

```c
if (p == NULL) {
    errno = EINVAL;
    return -1;
}
usa(*p);
```

Nel C "difensivo" tipico delle librerie, ogni funzione pubblica verifica i puntatori. Internamente, se hai contratti chiari (precondizioni), puoi rilassarti.

## Sanitizer — il tuo migliore amico

```bash
gcc -std=c17 -g -O1 -fsanitize=address,undefined -Wall -Wextra sorgente.c -o p
./p
```

AddressSanitizer (ASan) e UndefinedBehaviorSanitizer (UBSan) trovano:
- out-of-bounds,
- use-after-free,
- dereferenze NULL,
- overflow signed,
- shift invalidi.

**In fase di apprendimento, compila TUTTO con queste flag**.

## Esercizi

1. Scrivi un programma che stampa `argv` riga per riga.
2. Implementa `char *mia_strcpy(char *restrict dst, const char *restrict src)`.
3. Scrivi `void rimuovi_elemento(int *a, size_t *n, size_t idx)` che rimuove l'elemento all'indice `idx` e aggiorna `n` tramite puntatore.
4. Conta le occorrenze di un carattere in una stringa usando solo puntatori (niente `[]`).
5. Compila un tuo programma con `-fsanitize=address` e introduci volontariamente un bug (out-of-bounds). Leggi il report.
