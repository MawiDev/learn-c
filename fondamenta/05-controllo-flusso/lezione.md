# Lezione 05 — Controllo di flusso

## `if` / `else if` / `else`

```c
if (condizione) {
    ...
} else if (altra) {
    ...
} else {
    ...
}
```

Regole di stile del corso:

1. **Sempre le parentesi graffe**, anche per un singolo statement:
   ```c
   if (x > 0)
       printf("...");     // no, prima o poi qualcuno aggiunge una riga
   ```
   Bug famoso: [Apple `goto fail;`](https://www.imperialviolet.org/2014/02/22/applebug.html). Un `if` senza graffe ha rotto SSL in iOS.

2. **Condizioni esplicite**: `if (p != NULL)` è più chiaro di `if (p)`. In contesti numerici, `if (x != 0)` batte `if (x)`.

3. **Niente `yoda conditions`** se usi `-Wall`: il compilatore ti avverte su `if (x = 5)`.

## Verità e falsità in C

In C:
- `0`, `NULL`, `'\0'`, `0.0`, `false` → falsi
- tutto il resto → vero

`if (ptr)` equivale a `if (ptr != NULL)`. Funziona, ma l'esplicito è più chiaro, specie per i principianti.

## `switch`

Per confronti uguaglianza tra un `int` (o carattere, o enum) e valori costanti:

```c
switch (comando) {
    case 'q':
        printf("quit\n");
        break;
    case 'h':
    case '?':
        printf("help\n");
        break;
    default:
        printf("sconosciuto\n");
        break;
}
```

Punti critici:

1. **`break`** o cadrai nel caso successivo ("fallthrough"). A volte è voluto, ma segnalalo: in C23 esiste `[[fallthrough]];` ufficiale, in GCC/Clang `__attribute__((fallthrough));` o il commento `/* fall through */`.
2. `switch` funziona **solo su tipi interi** (`int`, `char`, `enum`). Niente `double`, niente stringhe.
3. `default` va sempre gestito, anche solo con un `break`. Rende le intenzioni chiare.

## Esempio con enum (tipico in C moderno)

```c
typedef enum { STATO_IDLE, STATO_RUN, STATO_STOP } Stato;

void tick(Stato s) {
    switch (s) {
        case STATO_IDLE: /* ... */ break;
        case STATO_RUN:  /* ... */ break;
        case STATO_STOP: /* ... */ break;
    }
}
```

Se ometti un caso dell'enum, `-Wswitch` te lo segnala: l'enum diventa un contratto.

## `goto` — esiste, ma...

Sì, esiste. No, non lo usi per controllo di flusso generico. L'unico uso idiomatico in C:

```c
FILE *f = NULL;
char *buf = NULL;

f = fopen(path, "r");
if (!f) goto err;

buf = malloc(N);
if (!buf) goto err;

/* ... usa f e buf ... */

free(buf);
fclose(f);
return 0;

err:
    if (buf) free(buf);
    if (f)   fclose(f);
    return -1;
```

Questa "single cleanup at exit" è uno **stile kernel Linux**. Cristallino per la gestione delle risorse in funzioni lunghe. Altri usi di `goto` sono da evitare.

## Esercizi

1. Scrivi un programma che legge un voto da 0 a 30 e stampa "ottimo", "buono", "sufficiente", "insufficiente" usando `if`/`else if`.
2. Riscrivilo con `switch` (hint: dividi il voto per una soglia).
3. Simula una macchina a stati: stati `IDLE → RUN → STOP`. Leggi un comando (`r`=run, `s`=stop, `q`=quit) e stampa lo stato.
4. Implementa `fizzbuzz`: per i numeri 1..100 stampa "Fizz" se divisibile per 3, "Buzz" per 5, "FizzBuzz" per 15, altrimenti il numero.
