# Lezione 08 — Array

Un **array** è una sequenza contigua di elementi dello stesso tipo.

## Dichiarazione

```c
int numeri[5];                     // non inizializzato
int numeri[5] = {1, 2, 3, 4, 5};   // inizializzato
int zeri[100] = {0};               // {0} riempie di zeri tutto
int parziale[5] = {1, 2};          // {1, 2, 0, 0, 0}
int inferito[] = {10, 20, 30};     // dimensione dedotta: 3
```

La dimensione **fa parte del tipo**: `int[5]` è un tipo distinto da `int[6]`.

### Designated initializers (C99+)

```c
int priorita[10] = { [3] = 7, [7] = 9 };    // gli altri sono 0
```

Elegante per tabelle sparse.

## Accesso agli elementi

```c
numeri[0]   // primo
numeri[4]   // ultimo (indice 4 per array di dimensione 5)
```

**Gli indici partono da 0**. L'indice dell'ultimo è `dim - 1`.

### Out-of-bounds = Undefined Behavior

```c
int a[5];
a[5] = 42;   // UB: l'indice valido va da 0 a 4
a[-1] = 0;   // UB
```

Il C **non** controlla i bounds. È tua responsabilità. Strumenti come `-fsanitize=address` (AddressSanitizer) ti aiutano in debug.

## Dimensione a tempo di compilazione

```c
int a[10];
size_t n = sizeof a / sizeof a[0];   // n = 10
```

**Importante**: `sizeof arr / sizeof arr[0]` funziona solo sull'array "vero", non su un puntatore. Quando passi un array a una funzione, diventa un puntatore (*array decay*) e perdi questa informazione.

C23 introduce `nelementsof`/`countof` ma non ci contare ancora; nel frattempo puoi definire:

```c
#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
```

## Array come parametro di funzione

```c
void stampa(int a[], size_t n)          // equivalente a int *a
void stampa(int a[10], size_t n)        // il 10 è ignorato!
void stampa(int *a, size_t n)           // stessa cosa
```

**Sempre passa anche la dimensione**. Nel C idiomatico il puntatore e la lunghezza viaggiano insieme.

```c
void stampa(const int *a, size_t n) {
    for (size_t i = 0; i < n; ++i) printf("%d ", a[i]);
}
```

Nota il `const`: diciamo al compilatore e a chi legge che la funzione **non modifica** l'array.

## Array multidimensionali

```c
int m[3][4];                    // 3 righe, 4 colonne
int m[3][4] = {
    {1, 2, 3, 4},
    {5, 6, 7, 8},
    {9, 10, 11, 12}
};

printf("%d\n", m[1][2]);        // 7
```

In memoria sono **row-major**: `m[1][2]` è l'elemento in posizione `1*4 + 2 = 6` dall'inizio.

Passaggio a funzione:

```c
void stampa(int rows, int cols, int m[rows][cols]) { ... }  // VLA-style (C99)
```

## VLA — Variable Length Array (C99, opzionale in C11+)

```c
void f(size_t n) {
    int buf[n];   // VLA: dimensione determinata a runtime
    ...
}
```

Comodo, ma:
- finiscono sullo stack → con `n` grande, crash.
- sono diventati **opzionali** dal C11.
- vietati nel Linux kernel, sconsigliati in sistemi embedded.

In pratica: evitali. Usa `malloc` per dimensioni grandi o non note.

## Array di caratteri = stringa

```c
char s[6] = "Ciao!";    // 5 caratteri + '\0' finale
```

Dedicheremo la prossima lezione alle stringhe.

## Esercizi

1. Dichiara un array di 10 `int` e riempilo con i quadrati 0..81.
2. Scrivi `int somma(const int *a, size_t n)` e testalo.
3. Scrivi `size_t indice_max(const int *a, size_t n)` che ritorna l'indice dell'elemento massimo.
4. Inverti un array in-place (senza array ausiliari).
5. Crea una matrice 5x5 e stampala formattata a tabella.
