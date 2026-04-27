# Lezione 06 (medi) — Preprocessore e macro

Il preprocessore gira **prima** del compilatore. Non capisce il C, fa solo sostituzioni testuali. Ma è potente e rischioso.

## Direttive fondamentali

| Direttiva  | Uso |
|------------|-----|
| `#include` | includi un file |
| `#define`  | definisci una macro |
| `#undef`   | cancella una macro |
| `#if` / `#elif` / `#else` / `#endif` | compilazione condizionale |
| `#ifdef` / `#ifndef` | abbreviazioni comuni |
| `#error`   | errore di compilazione forzato |
| `#warning` | warning (estensione ampiamente supportata) |
| `#pragma`  | istruzioni specifiche del compilatore |

## `#include`

```c
#include <stdio.h>      // header di sistema
#include "mylib.h"      // header locale (cerca prima nella dir del sorgente)
```

## `#define` — costanti testuali

```c
#define MAX_LEN 128
#define PI 3.14159
```

Nel C moderno, per costanti **semplici**, preferisci `const` o `enum`:
```c
static const int MAX_LEN = 128;
enum { BUFFER_SIZE = 4096 };
```

Perché?
- `const` ha un tipo, si può usare nel debugger, rispetta lo scope.
- `#define` è una macro: se qualcuno redefinisce `MAX_LEN` come `"ciao"` ovunque nel file, sei fregato.

`#define` resta insostituibile per:
- macro parametriche,
- compilazione condizionale,
- stringhe nel preprocessore (con `#` e `##`).

## Macro parametriche

```c
#define SQUARE(x) ((x) * (x))
```

Le parentesi sono **obbligatorie**, altrimenti:

```c
#define SQUARE(x) x * x
SQUARE(2 + 3)     // diventa 2 + 3 * 2 + 3 = 11, non 25!
```

Regola: **parentesi attorno a ogni argomento e attorno all'intero risultato**.

Altro problema: side effect. `SQUARE(i++)` diventa `((i++) * (i++))` → UB.

## `MIN`/`MAX` e il problema della doppia valutazione

```c
#define MAX(a, b) ((a) > (b) ? (a) : (b))
MAX(f(), g())      // f() chiamata DUE VOLTE
```

Soluzione moderna: usa una funzione `static inline`:

```c
static inline int max_int(int a, int b) { return a > b ? a : b; }
```

Le macro le userai quando servono davvero.

## Stringification `#` e token-pasting `##`

```c
#define STR(x)  #x
#define CAT(a, b) a ## b

printf("%s\n", STR(ciao));       // "ciao"
int CAT(var, 1) = 5;             // diventa: int var1 = 5;
```

Utili in macro di debug e generazione di codice.

## Compilazione condizionale

```c
#if defined(DEBUG) && DEBUG
    #define LOG(...) fprintf(stderr, __VA_ARGS__)
#else
    #define LOG(...) ((void)0)
#endif
```

Fondamentale per cross-platform:
```c
#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif
```

E per evitare inclusione multipla di header (**include guard**):

```c
#ifndef MYLIB_H
#define MYLIB_H

/* contenuto dell'header */

#endif
```

O, in quasi tutti i compilatori moderni:
```c
#pragma once
```

Non è standard ma funziona con GCC, Clang, MSVC. È più breve; ma **l'include guard è portabile al 100%**.

## Variadic macro

```c
#define LOG(fmt, ...) fprintf(stderr, fmt "\n", __VA_ARGS__)
```

`__VA_ARGS__` è la lista di argomenti extra. In C99 hai bisogno di almeno un argomento extra; in C23 (e come estensione GCC/Clang) `__VA_OPT__` risolve il caso con zero argomenti.

## Macro predefinite utili

| Macro | Contenuto |
|-------|-----------|
| `__FILE__` | nome del file sorgente (stringa) |
| `__LINE__` | numero di riga (int) |
| `__func__` | nome della funzione corrente (C99+) |
| `__DATE__` | data di compilazione |
| `__TIME__` | ora di compilazione |
| `__STDC_VERSION__` | versione dello standard (201710L per C17) |

Utili per logging:
```c
#define LOG(fmt, ...) \
    fprintf(stderr, "%s:%d [%s] " fmt "\n", \
            __FILE__, __LINE__, __func__, __VA_ARGS__)
```

## Quando NON usare macro

- Per sostituire una funzione semplice che accetta tipi ben noti.
- Per nascondere controllo di flusso (`#define RETURN_IF_NULL(p) if (!p) return -1;` è subdolo).
- Per accorciare nomi fino all'incomprensibilità.

## `#pragma`

Direttive specifiche del compilatore:
```c
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
int x;
#pragma GCC diagnostic pop
```

Usale con parsimonia, preferibilmente per silenziare un warning specifico e motivato.

## Esercizi

1. Scrivi un header `mylib.h` con include guard. Definisci una macro `ARRAY_SIZE`.
2. Scrivi una macro variadica `LOG(fmt, ...)` che include `__FILE__:__LINE__` nel messaggio.
3. Scrivi una macro `SWAP(a, b, type)` che scambia due valori via variabile temporanea locale. Testa su `int`, `double`, puntatore.
4. Usa `_Static_assert` (C11) per verificare a tempo di compilazione che `sizeof(int) >= 4`.
5. Dimostra il danno di una macro mal-parentesizzata: scrivi `SQUARE(x) x*x` e usala con `SQUARE(1+2)`.
