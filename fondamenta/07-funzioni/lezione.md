# Lezione 07 — Funzioni

Una funzione è una **unità di codice riusabile** con input (parametri) e output (valore di ritorno).

## Anatomia

```c
tipo_di_ritorno nome_funzione(tipo1 param1, tipo2 param2) {
    corpo;
    return valore;
}
```

Esempio:

```c
int somma(int a, int b) {
    return a + b;
}
```

## Dichiarazione vs definizione

- **Dichiarazione (prototipo)**: dice al compilatore tipo e firma.
  ```c
  int somma(int a, int b);
  ```
- **Definizione**: contiene anche il corpo.
  ```c
  int somma(int a, int b) { return a + b; }
  ```

In C **devi dichiarare una funzione prima di usarla**. Le due opzioni:
1. definirla prima dell'uso nel file,
2. mettere il prototipo in cima (o in un `.h`).

Senza prototipo il compilatore C "indovina" (regole legacy): nel C moderno è un errore. Usa `-Wall -Wstrict-prototypes`.

## `void`

- `void nomefunz(...)` → non ritorna nulla.
- `int nomefunz(void)` → non prende parametri. **Scrivi sempre `void`**, non lasciare `()` vuoto. `()` in C significa "argomenti non specificati", una regola storica oggi pericolosa.

## Passaggio per valore

In C **tutti i parametri sono passati per valore**. La funzione riceve una **copia**:

```c
void incr(int x) {
    x = x + 1;
}

int main(void) {
    int n = 5;
    incr(n);
    printf("%d\n", n);   // stampa 5, NON 6
}
```

Per "modificare" un argomento, passi un **puntatore** (lezione 10):

```c
void incr(int *x) {
    *x = *x + 1;
}

int n = 5;
incr(&n);
printf("%d\n", n);   // stampa 6
```

## Variabili locali

Le variabili dichiarate dentro una funzione sono **locali**: vivono sullo **stack**, nascono all'ingresso, muoiono all'uscita. Non persistono tra chiamate.

### `static` in una funzione

```c
int contatore(void) {
    static int n = 0;   // inizializzata UNA sola volta
    return ++n;
}
```

`static` rende la variabile persistente tra chiamate. **Non rende la funzione thread-safe**: occhio.

## `static` sulla funzione

```c
static int helper(int x) { ... }
```

Significa "visibile solo in questo file (.c)". Nascondi dettagli interni. Analogo a `private` in altri linguaggi.

## Regola d'oro delle funzioni

> Una funzione deve fare **una cosa** e farla bene.

Segnali che è il momento di spezzarla:
- è più lunga di uno schermo,
- ha più di 4-5 parametri,
- ha più di 3 livelli di nidificazione,
- il nome è una congiunzione ("read_and_parse_and_validate").

## Ritornare più valori

Il C non ti dà tuple. Alternative:

1. **Puntatori di output**:
   ```c
   int dividi(int a, int b, int *quoziente, int *resto);
   ```
2. **Struct**:
   ```c
   typedef struct { int q, r; } DivResult;
   DivResult dividi(int a, int b);
   ```

Il #2 è spesso più pulito nel C moderno.

## Funzioni variadiche

`printf` è variadica. Puoi scriverne anche tu con `<stdarg.h>`:

```c
#include <stdarg.h>

int somma(int n, ...) {
    va_list ap;
    va_start(ap, n);
    int tot = 0;
    for (int i = 0; i < n; ++i) tot += va_arg(ap, int);
    va_end(ap);
    return tot;
}
```

Lo useremo poco: sono meno type-safe.

## Ricorsione

```c
long long fatt(int n) {
    if (n <= 1) return 1;
    return n * fatt(n - 1);
}
```

Funziona. Ma lo stack del C è finito (~1-8 MB): una ricorsione profonda → stack overflow. Preferisci l'iterativo quando possibile.

## Esercizi

1. Scrivi `int abs_int(int x)` senza usare `<stdlib.h>`.
2. Scrivi una funzione `bool e_primo(int n)` e usala in un loop che stampa i primi 50 numeri primi.
3. Scrivi `void scambia(int *a, int *b)` che scambia due interi.
4. Scrivi `DivResult dividi(int a, int b)` che restituisce quoziente e resto in una struct.
5. Implementa Fibonacci ricorsivo e iterativo. Misura il tempo per `n = 40` con `time(1)`: noterai perché la ricorsione senza memoization è disastrosa.
