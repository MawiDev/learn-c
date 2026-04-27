# Lezione 06 — Cicli

## `while`

```c
while (condizione) {
    corpo;
}
```

Valuta la condizione **prima** di ogni iterazione. Se è falsa al primo controllo, il corpo non viene mai eseguito.

## `do ... while`

```c
do {
    corpo;
} while (condizione);     // il punto e virgola qui è OBBLIGATORIO
```

Esegue il corpo **almeno una volta**. Utile per input validati da utente.

## `for` — il re dei cicli

```c
for (inizializzazione; condizione; aggiornamento) {
    corpo;
}
```

Equivalente a:
```c
inizializzazione;
while (condizione) {
    corpo;
    aggiornamento;
}
```

### Dichiarazione nel for (C99+)

```c
for (int i = 0; i < 10; ++i) { ... }
```

`i` esiste **solo dentro il for**. Non inquina lo scope esterno. Usa sempre questa forma.

### Scelta del tipo dell'indice

Per iterare su array:
```c
size_t n = ...;
for (size_t i = 0; i < n; ++i) { ... }
```

`size_t` è il tipo "ufficiale" delle dimensioni in C. È **unsigned**. `-Wsign-compare` ti avvisa se confronti `int` con `size_t`.

> Attenzione: `for (size_t i = n - 1; i >= 0; --i)` è un ciclo **infinito**: `size_t` è unsigned e non scende mai sotto 0. Va scritto così:
> ```c
> for (size_t i = n; i-- > 0; ) { /* usa i */ }
> ```

## `break` e `continue`

- `break` → esce immediatamente dal ciclo (o dallo `switch`) più interno.
- `continue` → salta al prossimo controllo della condizione.

Usali con parsimonia. Un ciclo con 5 `break` e 3 `continue` si riscrive quasi sempre meglio.

## Cicli infiniti

```c
while (1)       { ... }
for (;;)        { ... }
```

Entrambi equivalenti. `for (;;)` è l'idioma C classico. Dentro userai `break` o `return` per uscire.

## Anti-pattern da evitare

```c
int i = 0;
while (i < n) {
    fai_qualcosa(arr[i]);
    i++;
}
```

Funziona, ma è un `for` mascherato. Scrivilo come `for`.

```c
for (int i = 0; i < 10; i++) {
    if (i == 3) continue;
    if (i == 7) break;
    if (i % 2) continue;
    ...
}
```

Troppi salti. Riscrivi la logica: la leggibilità vince.

## Nidificazione

Per uscire da cicli nidificati non esistono `break N` come in altri linguaggi. Puoi:

1. usare una `bool` come flag,
2. usare una **funzione** e fare `return`,
3. usare `goto` verso un label di uscita (idiomatico per cleanup).

```c
bool trovato = false;
for (size_t i = 0; i < rows && !trovato; ++i) {
    for (size_t j = 0; j < cols && !trovato; ++j) {
        if (matrice[i][j] == target) trovato = true;
    }
}
```

## Esercizi

1. Stampa i primi 20 numeri di Fibonacci con un `for`.
2. Leggi numeri da stdin finché l'utente non inserisce 0, poi stampa somma e media.
3. Scrivi un ciclo che decrementa un `size_t` da `n-1` a `0` in modo sicuro.
4. Trova il primo numero primo maggiore di 1000.
5. Stampa una tavola pitagorica 10x10 con cicli annidati.
