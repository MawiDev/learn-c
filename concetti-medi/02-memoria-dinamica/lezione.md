# Lezione 02 (medi) — Memoria dinamica

## Stack vs Heap

| Stack                           | Heap                            |
|---------------------------------|---------------------------------|
| variabili locali                | memoria allocata con `malloc`   |
| dimensione fissa e piccola      | dimensione grande (GB)          |
| automatica: nasce/muore con la funzione | manuale: tu la allochi e liberi |
| velocissimo                     | più lento                       |

Quando non sai a compile-time **quanto** spazio ti serve, o quando un oggetto deve **sopravvivere** alla funzione che lo ha creato, usi il **heap**.

## Le quattro funzioni di `<stdlib.h>`

```c
void *malloc(size_t size);
void *calloc(size_t n, size_t size);   // n * size, AZZERATA
void *realloc(void *ptr, size_t size); // ridimensiona (o sposta)
void  free(void *ptr);                 // libera
```

## `malloc` — il caso base

```c
int *a = malloc(10 * sizeof *a);
if (a == NULL) { /* errore: memoria esaurita */ }

for (int i = 0; i < 10; ++i) a[i] = i;

free(a);
a = NULL;          // buona abitudine: evita use-after-free accidentali
```

Idioma **fortemente raccomandato**: `sizeof *a` invece di `sizeof(int)`.

Se un giorno cambi `a` da `int *` a `long *`, il codice continua a essere corretto senza toccare il `sizeof`. È uno dei segni di codice C ben scritto.

## `calloc` vs `malloc`

- `malloc(n)` → n byte, **contenuto indeterminato** (spazzatura).
- `calloc(k, n)` → `k * n` byte, **azzerati**.

`calloc` ha anche protezione contro overflow su `k * n`.

## `realloc` — ridimensionare

```c
int *a = malloc(N * sizeof *a);
...

int *tmp = realloc(a, 2 * N * sizeof *a);
if (tmp == NULL) {
    // ATTENZIONE: a è ancora valido, la realloc ha fallito
    free(a);
    return NULL;
}
a = tmp;
```

**Mai** fare `a = realloc(a, ...)`: se fallisce, perdi il puntatore originale e hai un memory leak.

`realloc(NULL, n)` equivale a `malloc(n)`. `realloc(p, 0)` è **implementation-defined** in C23 (in passato ha liberato `p` e ritornato `NULL`, ma non è più garantito): non farlo.

## `free` — regole

```c
free(p);
```

- Puoi passare `NULL` a `free`: è legale e non fa nulla.
- **Non** fare `free` due volte sullo stesso puntatore: **double-free** → UB, spesso sfruttabile come vulnerabilità.
- **Non** usare un puntatore dopo averlo `free`-ato: **use-after-free**.

Pattern pulito:
```c
free(p);
p = NULL;
```

## Regola d'oro

> **Ogni `malloc` deve avere un `free` corrispondente**, lungo ogni cammino di esecuzione (inclusi gli errori).

Il `goto cleanup` visto nella lezione di controllo di flusso è perfetto per questo.

## Strutture dinamiche — esempio: vettore ridimensionabile

```c
typedef struct {
    int    *data;
    size_t  len;
    size_t  cap;
} Vector;

bool vec_push(Vector *v, int x) {
    if (v->len == v->cap) {
        size_t ncap = v->cap ? v->cap * 2 : 4;
        int *tmp = realloc(v->data, ncap * sizeof *tmp);
        if (!tmp) return false;
        v->data = tmp;
        v->cap  = ncap;
    }
    v->data[v->len++] = x;
    return true;
}

void vec_free(Vector *v) {
    free(v->data);
    v->data = NULL;
    v->len = v->cap = 0;
}
```

Questa è l'anatomia di uno `std::vector` del C++, scritta a mano. Il C ti costringe a vederla.

## Strumenti per memoria

- **Valgrind** (Linux): `valgrind --leak-check=full ./programma`
- **AddressSanitizer**: `-fsanitize=address` (trova leak, use-after-free, out-of-bounds).

Usali ad ogni progetto. Non c'è scusa.

## Allineamento (cenno)

`malloc` restituisce memoria allineata al tipo massimo "fondamentale" della piattaforma. Per allineamenti speciali (SIMD, hardware), C11 offre:

```c
#include <stdlib.h>
void *p = aligned_alloc(64, N);    // allineato a 64 byte
```

## Esercizi

1. Scrivi `int *duplica(const int *src, size_t n)` che alloca un nuovo array, copia, e ritorna il puntatore. Il chiamante libererà.
2. Implementa il `Vector` mostrato sopra, con `vec_init`, `vec_push`, `vec_get`, `vec_free`.
3. Scrivi `char *leggi_riga(FILE *f)` che legge una riga di lunghezza sconosciuta usando `realloc`.
4. Introduci volontariamente un double-free e osserva il comportamento sotto ASan.
5. Misura la differenza di tempo fra `malloc(1) / free` ripetuto 1M volte e `calloc(1, 1)`.
