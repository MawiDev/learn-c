# Lezione 07 (medi) — Puntatori a funzione

Le funzioni in C hanno un indirizzo. Puoi memorizzarlo in un puntatore e chiamarlo dopo.

## Sintassi

```c
int somma(int a, int b) { return a + b; }

int (*pf)(int, int);    // puntatore a funzione (int,int)->int
pf = somma;             // "&somma" è equivalente
int r = pf(2, 3);       // "(*pf)(2, 3)" equivalente
```

Nota: le parentesi intorno a `*pf` sono obbligatorie. Senza: `int *pf(int, int)` è la dichiarazione di una funzione che ritorna `int *`.

## Chiarire con `typedef`

```c
typedef int (*BinOp)(int, int);

int somma(int a, int b) { return a + b; }
int diff (int a, int b) { return a - b; }

BinOp op = somma;
printf("%d\n", op(3, 4));
op = diff;
printf("%d\n", op(3, 4));
```

**Molto più leggibile**. Usa sempre `typedef` per firme ripetute.

## Tabelle di dispatch

Pattern classico: scegli una funzione in base a una chiave.

```c
typedef int (*Handler)(int arg);

typedef struct {
    const char *nome;
    Handler     fn;
} Cmd;

static int aiuto(int a)   { (void)a; printf("aiuto\n");   return 0; }
static int versione(int a){ (void)a; printf("v1.0\n");   return 0; }
static int quit(int a)    { (void)a; return 42; }

static const Cmd tabella[] = {
    { "help",    aiuto },
    { "version", versione },
    { "quit",    quit },
};
```

Più pulito di un `if/else if` lungo. Molti parser, shell, protocolli funzionano così.

## `qsort` — l'esempio canonico

`qsort` in `<stdlib.h>` ordina un array generico. Non sa cosa c'è dentro: glielo dici tu con un comparatore.

```c
void qsort(void *base, size_t n, size_t size,
           int (*cmp)(const void *, const void *));

int cmp_int(const void *a, const void *b) {
    int ia = *(const int*)a;
    int ib = *(const int*)b;
    return (ia > ib) - (ia < ib);   // evita overflow di (ia - ib)
}

int a[] = {5, 2, 8, 1, 9};
qsort(a, 5, sizeof a[0], cmp_int);
```

Il trucco `(x > y) - (x < y)`: ritorna -1, 0 o 1 senza rischi di overflow.

## Callback

Una funzione "parametrica" che accetta una callback. Molto usato per astrazioni tipo `foreach`.

```c
void for_each(int *a, size_t n, void (*fn)(int *)) {
    for (size_t i = 0; i < n; ++i) fn(&a[i]);
}

void doppio(int *x) { *x *= 2; }

int a[] = {1, 2, 3};
for_each(a, 3, doppio);
```

In C il callback è **solo il puntatore**: non puoi catturare variabili locali come una closure. Per passare "contesto", aggiungi un parametro `void *user_data`:

```c
typedef void (*Callback)(void *ctx, int *x);

void for_each(int *a, size_t n, Callback fn, void *ctx) {
    for (size_t i = 0; i < n; ++i) fn(ctx, &a[i]);
}
```

Questo è **lo** pattern del C moderno per i callback.

## Array di puntatori a funzione

```c
typedef void (*Stato)(void);

void idle(void)  { printf("idle\n"); }
void run (void)  { printf("run\n"); }
void stop(void)  { printf("stop\n"); }

static const Stato stati[] = { idle, run, stop };
stati[1]();   // chiama run
```

## Lettura della sintassi ostica (spiral rule)

```c
void (*signal(int, void (*)(int)))(int);
```

Si legge con la **regola della spirale**: parti dal nome, vai a destra finché puoi, poi a sinistra, usando le parentesi come limiti. È un puntatore a funzione che prende `int` e ritorna `void`, ottenuto chiamando `signal` con `int` e un altro puntatore-a-funzione uguale.

Traduzione moderna con `typedef`:
```c
typedef void (*SigHandler)(int);
SigHandler signal(int signum, SigHandler handler);
```

Vedi la differenza? Usa sempre i typedef.

## Esercizi

1. Scrivi `qsort` su un array di stringhe (`char **`) con `strcmp`.
2. Scrivi una tabella comando → handler e fai partire una piccola shell: l'utente inserisce `help`, `version`, `quit`.
3. Scrivi `int trasforma_e_somma(const int *a, size_t n, int (*f)(int))` che applica `f` a ogni elemento e ritorna la somma dei risultati.
4. Implementa uno state machine con array di puntatori a funzione, uno stato per funzione.
5. Scrivi `for_each` con parametro `void *ctx` e usalo per incrementare ogni elemento di un `delta` passato come contesto.
