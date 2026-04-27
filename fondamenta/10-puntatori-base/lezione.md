# Lezione 10 — Puntatori (le basi)

I puntatori sono il **cuore** del C. Se li capisci, capisci il C.

## Cos'è un puntatore

Ogni variabile occupa uno o più byte in memoria, a un certo **indirizzo**. Un **puntatore** è una variabile che contiene l'indirizzo di un'altra variabile.

```c
int x = 42;     //  x vive a qualche indirizzo, es. 0x7ffe...a0
int *p = &x;    //  p contiene quell'indirizzo
```

- `&x` → operatore **address-of**: "dammi l'indirizzo di x".
- `int *p` → "p è un puntatore a int".
- `*p` → operatore **dereference**: "dammi il valore puntato da p".

```c
*p = 100;       // modifica x tramite p: ora x == 100
```

## Pensa a memoria come nastro

```
indirizzi: 0x1000  0x1004  0x1008
valori  :  [ 42 ] [ ... ] [ ... ]
             ^
             x

p  →  0x1000       // p contiene l'indirizzo di x
*p →  42           // dereferenzio → leggo il valore a 0x1000
```

## Dichiarazione: attenzione alla sintassi

```c
int *p1, *p2;    // due puntatori a int
int *p1, p2;     // p1 è puntatore, p2 è un int!
```

Il `*` si lega **al nome**, non al tipo. Stile raccomandato: `int *p`. Se ti confonde, dichiara su righe separate.

## `NULL` — il puntatore a nulla

```c
int *p = NULL;   // p non punta a nulla di valido
```

`NULL` è una macro (`((void*)0)` tipicamente). In C23 puoi usare `nullptr`.

**Mai dereferenziare un NULL**:
```c
int x = *p;   // p == NULL → UB → segfault
```

Il controllo idiomatico:
```c
if (p != NULL) {
    usa(*p);
}
```

## Puntatori e funzioni

Finora hai visto il passaggio per valore. I puntatori abilitano il passaggio **per riferimento**:

```c
void incr(int *n) {
    *n = *n + 1;
}

int main(void) {
    int a = 5;
    incr(&a);
    printf("%d\n", a);   // 6
}
```

## Puntatori e array — il legame

Un array "decade" in un puntatore al primo elemento quando passato a una funzione:

```c
int a[5] = {10, 20, 30, 40, 50};
int *p = a;        // equivalente a &a[0]

printf("%d\n", a[2]);    // 30
printf("%d\n", p[2]);    // 30
printf("%d\n", *(p+2));  // 30
```

**Le tre scritture sono equivalenti**. Questo è fondamentale.

### Aritmetica dei puntatori

Quando aggiungi `n` a un puntatore di tipo `T*`, l'indirizzo avanza di `n * sizeof(T)` byte.

```c
int a[5];
int *p = a;
p++;    // ora punta ad a[1] (avanza di sizeof(int) byte)
```

Per questo `p[i]` è definito come `*(p + i)`: il compilatore sa la dimensione del tipo.

## `const` e puntatori — regola fondamentale

Tre combinazioni, tutte diverse:

```c
const int *p;         // p punta a un int const: non puoi modificare *p
                      // puoi far puntare p altrove

int *const p;         // p è const: *p modificabile, p no

const int *const p;   // entrambi const
```

Lettura da destra a sinistra: `int *const` → "const pointer to int". `const int *` → "pointer to const int".

Negli argomenti di funzione, usa `const T *` quando non modifichi i dati. È documentazione e vincolo per il compilatore:

```c
size_t mia_strlen(const char *s);
```

## `void *` — puntatore generico

```c
void *p;
```

Un `void *` può puntare a qualsiasi tipo. Non puoi dereferenziarlo direttamente (non sai la dimensione). Si usa per:
- API generiche: `malloc` ritorna `void *`.
- `memcpy(void *dst, const void *src, size_t n)`.

Prima di usarlo, lo converti:
```c
int *pi = (int *)malloc(sizeof *pi);
```

In C (a differenza del C++), il cast da `void *` è implicito e può essere omesso; nel corso lo esplicitiamo per chiarezza pedagogica, ma è dibattuto.

## Errori classici

1. **Dereferenza di NULL**:
   ```c
   int *p = NULL;
   *p = 5;   // crash
   ```

2. **Dangling pointer** (puntatore a memoria non più valida):
   ```c
   int *p;
   {
       int x = 5;
       p = &x;
   }
   *p = 0;   // x non esiste più: UB
   ```

3. **Uninitialized pointer**:
   ```c
   int *p;     // punta a chissà dove
   *p = 5;    // UB
   ```

Per tutti e tre: **inizializza a `NULL`**, e controlla prima di dereferenziare.

## Esercizi

1. Scrivi `void scambia(int *a, int *b)` e usala.
2. Scrivi `void zero_array(int *a, size_t n)` usando aritmetica dei puntatori (senza `[]`).
3. Scrivi `int *cerca(int *a, size_t n, int target)` che restituisce il puntatore all'elemento, o `NULL` se non trovato.
4. Data una stringa, usa un puntatore che scorre con `++` per contare gli spazi.
5. Dimostra la differenza di comportamento tra `int *p` e `int *const p` cercando di riassegnarli.
