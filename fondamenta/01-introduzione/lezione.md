# Lezione 01 — Introduzione e compilazione

## Cos'è il C

Il C è un linguaggio **compilato**, **tipizzato staticamente**, e **a basso livello**.
Nasce nel 1972 con Dennis Ritchie. Gli standard moderni che useremo sono:

| Standard | Anno | Cosa introduce (riassunto) |
|----------|------|----------------------------|
| C89/C90  | 1989 | Il C "classico", ANSI |
| C99      | 1999 | `//` commenti, `bool`, VLA, `long long` |
| C11      | 2011 | Thread, `_Generic`, atomic, `_Static_assert` |
| C17      | 2018 | Bugfix di C11 (lo standard "di riferimento" oggi) |
| C23      | 2023 | `true`/`false` keyword, `nullptr`, attributi `[[...]]`, `typeof` |

In questo corso lavoreremo con **C17** come standard di default, ma ti farò vedere le novità di C23 dove rilevanti.

## Il tuo primo programma

```c
#include <stdio.h>

int main(void) {
    printf("Ciao, Riccardo!\n");
    return 0;
}
```

Analizziamolo riga per riga, come faresti leggendo una frase:

- `#include <stdio.h>` → direttiva al **preprocessore**. Include le dichiarazioni della libreria I/O standard (tra cui `printf`).
- `int main(void)` → la funzione `main` è il punto di ingresso del programma. Restituisce un `int` al sistema operativo. `(void)` significa "non riceve argomenti". **Scrivere `void` è importante**: `main()` senza `void` significa "argomenti non specificati", non "nessun argomento".
- `printf("...\n")` → stampa su standard output. `\n` è il carattere "a capo".
- `return 0;` → il programma termina con successo. Qualunque valore ≠ 0 indica errore.

## Il ciclo di compilazione

Il sorgente `.c` passa attraverso 4 fasi:

```
sorgente.c
    │
    ▼   preprocessore   (espande #include, #define)
sorgente.i
    │
    ▼   compilatore     (genera assembly)
sorgente.s
    │
    ▼   assembler       (codice macchina oggetto)
sorgente.o
    │
    ▼   linker          (unisce .o + librerie)
eseguibile
```

Normalmente `gcc` fa tutto in un colpo, ma puoi fermarti a ogni fase:

```bash
gcc -E hello.c           # solo preprocessore (output su stdout)
gcc -S hello.c           # genera hello.s
gcc -c hello.c           # genera hello.o
gcc hello.c -o hello     # fa tutto, produce eseguibile "hello"
```

## Flag che useremo SEMPRE

```bash
gcc -std=c17 -Wall -Wextra -Wpedantic -O2 hello.c -o hello
```

- `-std=c17` → standard del linguaggio.
- `-Wall -Wextra -Wpedantic` → attiva i warning. **Leggili sempre**. Il 90% dei bug si previene qui.
- `-O2` → ottimizzazione. In debugging userai `-O0 -g` per avere simboli utili al debugger.

## Errori vs Warning

- **Errore** → non compila. Devi sistemarlo.
- **Warning** → compila, ma il compilatore ti sta dicendo "sei sicuro?". Considerali errori. Aggiungi `-Werror` se vuoi che il compilatore ti forzi la mano.

## Esercizi

1. Compila ed esegui `esempio.c`.
2. Cambia il messaggio e ricompila.
3. Prova a compilare con `-Werror` un file che usa una variabile non inizializzata. Osserva come il compilatore ti difende.
4. Esegui `gcc -E esempio.c | less` e guarda cosa diventa il sorgente dopo il preprocessore (spoiler: enorme).
