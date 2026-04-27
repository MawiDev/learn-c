# Lezione 08 (medi) — Compilazione multi-file

Quando un programma cresce, lo dividi in più file. Capire come il C organizza codice tra file è fondamentale.

## Header (`.h`) vs Source (`.c`)

- **`.h`** — **interfaccia**. Dichiarazioni: prototipi di funzione, tipi, macro, `extern` di variabili globali. **Non** contiene definizioni (salvo `inline` e `static`).
- **`.c`** — **implementazione**. Definizioni delle funzioni, variabili globali reali.

Regola: **ogni `.c` include i `.h` di cui ha bisogno**, i `.h` includono a loro volta i propri `.h` di dipendenza.

## Esempio minimo

```
progetto/
├── main.c
├── matematica.c
└── matematica.h
```

**`matematica.h`**:
```c
#ifndef MATEMATICA_H
#define MATEMATICA_H

int somma(int a, int b);
int moltiplica(int a, int b);

#endif
```

**`matematica.c`**:
```c
#include "matematica.h"

int somma(int a, int b)       { return a + b; }
int moltiplica(int a, int b)  { return a * b; }
```

**`main.c`**:
```c
#include <stdio.h>
#include "matematica.h"

int main(void) {
    printf("%d\n", somma(2, 3));
    return 0;
}
```

Compilazione:
```bash
gcc -std=c17 -Wall -Wextra -Wpedantic main.c matematica.c -o prog
```

Oppure separatamente (più realistico):
```bash
gcc -c main.c          # genera main.o
gcc -c matematica.c    # genera matematica.o
gcc main.o matematica.o -o prog
```

Il vantaggio di separare: se tocchi solo `matematica.c`, ricompili solo `matematica.o`.

## Include guard — perché

Senza include guard, se `A.h` include `B.h` e `main.c` include entrambi, ti ritrovi `B.h` incluso due volte → errori di redefinizione.

```c
#ifndef MATEMATICA_H
#define MATEMATICA_H
/* contenuto */
#endif
```

`#pragma once` funziona in tutti i compilatori moderni ma non è standard. Usa include guard + nome unico (prefissa con il nome del progetto).

## `static` a livello file (linkage interno)

```c
// file.c
static int contatore = 0;
static void helper(void) { ... }
```

`static` qui significa **"privato al file"**: non visibile al linker, quindi non collidere con altri file.

**Ogni funzione che non esporti nell'header, dichiarala `static`**. È un'abitudine disciplinante.

## `extern` — variabili globali condivise

```c
// config.h
extern int verbose;

// config.c
int verbose = 0;
```

`extern` nell'header dice "esiste, ma è definito altrove". La definizione (senza `extern`) va in **un solo** `.c`.

Nota: le **variabili globali** sono spesso un anti-pattern. Se devi condividere stato, meglio passarlo esplicitamente o incapsularlo in funzioni accessor.

## Cosa mettere nell'header

✅ Prototipi di funzione
✅ Definizioni di tipo (`struct`, `typedef`, `enum`)
✅ `extern` di globali
✅ Macro
✅ `static inline` funzioni piccole (C99+)

❌ Definizioni di funzioni non-`inline` (duplicherebbero il simbolo)
❌ Definizioni di variabili globali non-`static` non-`const`

## One Definition Rule (informale)

Ogni funzione o variabile non-static ha **una e una sola** definizione nell'intero programma, ma può avere più dichiarazioni.

## Makefile minimo

```make
CC      = gcc
CFLAGS  = -std=c17 -Wall -Wextra -Wpedantic -O2 -g
OBJ     = main.o matematica.o
TARGET  = prog

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
```

Con `make` solo i file modificati vengono ricompilati. Per progetti grandi userai `cmake`, `meson` o altri.

## Linker errors — tradurre

- **undefined reference to 'somma'**: il prototipo esiste ma non c'è una definizione (o non hai passato il `.o` al linker).
- **multiple definition of 'somma'**: hai definito la funzione in più `.c`. Rendila `static` o sposta la definizione in uno solo.

## Esercizi

1. Crea la struttura `progetto/` dell'esempio, compila con gcc, poi scrivi un `Makefile`.
2. Aggiungi un file `str_utils.c/.h` con una funzione `char *trim(char *s)` che rimuove whitespace iniziale/finale. Usala in `main.c`.
3. Sposta la macro `ARRAY_SIZE` in un header `common.h` incluso da tutti.
4. Dimostra cosa succede se rimuovi l'include guard da `matematica.h` e lo includi due volte in `main.c`.
5. Scrivi una libreria statica (`libmatematica.a`) con `ar` e collegala: `gcc main.c -L. -lmatematica -o prog`.
