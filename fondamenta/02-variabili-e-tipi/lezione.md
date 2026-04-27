# Lezione 02 — Variabili e tipi

## Il concetto di tipo

In C ogni variabile ha un **tipo** fissato a tempo di compilazione. Il tipo determina:

1. **Quanti byte** occupa in memoria.
2. **Come vengono interpretati** quei byte (intero, float, puntatore...).
3. **Quali operazioni** sono lecite.

Il C non ti mente: se dichiari `int`, è un int per sempre.

## Tipi interi

| Tipo              | Dimensione minima | Note |
|-------------------|-------------------|------|
| `char`            | 1 byte            | Può essere signed o unsigned a seconda della piattaforma! |
| `signed char`     | 1 byte            | -128 … 127 |
| `unsigned char`   | 1 byte            | 0 … 255 |
| `short`           | ≥ 2 byte          | tipicamente 2 |
| `int`             | ≥ 2 byte          | tipicamente 4 |
| `long`            | ≥ 4 byte          | 4 su Windows, 8 su Linux 64-bit |
| `long long`       | ≥ 8 byte          | (C99) |

**Regola d'oro del C moderno**: quando la dimensione esatta conta, usa `<stdint.h>`:

```c
#include <stdint.h>

int8_t   a;   // esattamente 8 bit, con segno
uint8_t  b;   // esattamente 8 bit, senza segno
int32_t  c;   // esattamente 32 bit, con segno
uint64_t d;   // esattamente 64 bit, senza segno
```

Sono garantiti identici su ogni architettura. Usa questi quando scrivi protocolli di rete, formati binari, bit-twiddling.

## Tipi in virgola mobile

| Tipo          | Precisione tipica |
|---------------|-------------------|
| `float`       | 32 bit (7 cifre decimali) |
| `double`      | 64 bit (15 cifre decimali) |
| `long double` | ≥ 80 bit (dipende dalla piattaforma) |

**Default**: usa `double` salvo ragioni specifiche. `float` lo scegli per vincoli di memoria/performance (array enormi, GPU, DSP).

## Booleano

Prima del C99 non esisteva. Oggi:

```c
#include <stdbool.h>

bool trovato = true;
if (trovato) { ... }
```

In C23 `bool`, `true`, `false` diventano **keyword** del linguaggio — non serve più includere `<stdbool.h>`.

## Dichiarazione e inizializzazione

```c
int x;           // dichiarato, NON inizializzato → valore indeterminato!
int y = 42;      // dichiarato e inizializzato
int z = {42};    // ammesso (inizializzatore aggregato)
```

> **Regola di sopravvivenza**: **inizializza sempre** le variabili al momento della dichiarazione.
> Una variabile non inizializzata contiene "quello che c'era prima" in quella zona di memoria. È una delle cause più comuni di bug e vulnerabilità in C.

## `const` — la promessa che non modifichi

```c
const double PI = 3.14159265358979;
PI = 3.0;   // ERRORE: assignment of read-only variable 'PI'
```

`const` rende una variabile **non modificabile dopo l'inizializzazione**. Usalo ovunque possibile: il compilatore ti difende e genera codice migliore.

## Cast esplicito

Converte un valore da un tipo a un altro:

```c
int    a = 7;
int    b = 2;
double r1 = a / b;           // 3.0 (divisione intera, POI convertita)
double r2 = (double)a / b;   // 3.5 (a convertito PRIMA → div float)
```

Nel C "classico" si cede spesso a cast impliciti; nel C moderno preferiamo essere **espliciti**.

## Modificatori di dichiarazione

- `static` — vedremo a fondo in seguito. A livello file: limita la visibilità. Dentro a una funzione: la variabile sopravvive tra le chiamate.
- `extern` — dichiarazione di variabile definita altrove.
- `register` — obsoleto, ignoralo.
- `auto` — default, inutile scriverlo.

## Overflow e wraparound

```c
unsigned char c = 255;
c = c + 1;   // diventa 0 (wrap-around ben definito per unsigned)

int i = INT_MAX;
i = i + 1;   // COMPORTAMENTO INDEFINITO per signed!
```

Il C distingue:
- **unsigned**: l'overflow è definito (wrap modulo 2^N).
- **signed**: l'overflow è **UB (Undefined Behavior)**. Il compilatore può fare qualsiasi cosa, inclusa l'ottimizzazione aggressiva che rimuove i tuoi check.

## Esercizi

1. Scrivi un programma che stampi `sizeof` di `char`, `int`, `long`, `long long`, `float`, `double`, `void*` sulla tua macchina.
2. Usa `<stdint.h>` per dichiarare un `uint32_t` e stamparlo con la macro `PRIu32` da `<inttypes.h>`.
3. Prova a modificare una variabile `const`: leggi attentamente il messaggio di errore del compilatore.
4. Dimostra la differenza tra `int/int` e `(double)int/int`.
