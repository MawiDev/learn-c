# Lezione 03 (medi) — Strutture (`struct`)

Una `struct` raggruppa dati di tipi diversi in un singolo tipo aggregato.

## Dichiarazione

```c
struct Punto {
    double x;
    double y;
};

struct Punto p;
p.x = 1.0;
p.y = 2.0;
```

## `typedef` per nomi più puliti

```c
typedef struct {
    double x;
    double y;
} Punto;

Punto p = { .x = 1.0, .y = 2.0 };
```

Con il `typedef` scrivi `Punto` invece di `struct Punto`. È l'idioma del C moderno.

Se ti serve una struct auto-referenziale (lista, albero) il tag è obbligatorio:

```c
typedef struct Nodo {
    int           valore;
    struct Nodo  *next;
} Nodo;
```

## Designated initializer (C99+)

```c
Punto p = { .y = 2.0, .x = 1.0 };   // ordine libero, campi omessi = 0
```

Elegante, sicuro e documentante. Usalo sempre.

## Compound literal (C99+)

```c
sposta(&(Punto){ .x = 3, .y = 4 });   // struct anonima al volo
```

Utile per passare strutture ad hoc a funzioni senza nominarle.

## Accesso ai campi

- `.` → su un oggetto struct.
- `->` → scorciatoia per `(*p).campo` su un puntatore a struct.

```c
Punto p;
Punto *pp = &p;

p.x   = 1;
pp->y = 2;     // equivale a (*pp).y = 2;
```

## Passaggio a funzione

Due scelte:

```c
void stampa(Punto p);        // per valore: COPIA la struct
void stampa(const Punto *p); // per puntatore: no copia
```

Regola pratica:
- struct piccola (≤ ~16 byte) → per valore va bene.
- struct grande → per puntatore `const`.

Nei sistemi critici si preferisce sempre per puntatore const: mantiene il costo prevedibile.

## Ritornare una struct

```c
Punto origine(void) {
    return (Punto){ .x = 0, .y = 0 };
}
```

Il compilatore gestisce la copia (spesso con ottimizzazioni tipo RVO). Non è un problema per struct piccole.

## Padding e allineamento

```c
struct S {
    char  a;     // 1 byte
    int   b;     // 4 byte
    char  c;     // 1 byte
};
printf("%zu\n", sizeof(struct S));   // probabilmente 12, non 6!
```

Il compilatore inserisce **padding** tra i campi per mantenere l'allineamento. Conseguenze pratiche:

1. **Ordina i campi** dal più grande al più piccolo per minimizzare lo spreco:
   ```c
   struct { double d; int i; char c; };   // compatto
   ```
2. Per dump binario su file / rete, **non serializzare mai `memcpy` di una struct**: il padding ha contenuto indeterminato e varia per piattaforma.

In C23 puoi usare `_Alignas` e `_Alignof` (o `alignas`/`alignof` con `<stdalign.h>`) per controllare esplicitamente l'allineamento.

## `flexible array member` (C99+)

```c
typedef struct {
    size_t len;
    char   data[];   // membro "flessibile", deve essere l'ultimo
} Buffer;

Buffer *b = malloc(sizeof *b + N);
b->len = N;
memcpy(b->data, src, N);
```

Un solo `malloc` sia per l'header sia per il payload. Vedilo come il pattern "struct con array dinamico a lunghezza variabile".

## Bitfield (brevemente)

```c
struct Flags {
    unsigned ready : 1;
    unsigned error : 1;
    unsigned code  : 6;
};
```

Utile in protocolli low-level e driver. Portabilità limitata: ordine dei bit dipende dal compilatore. Non usarli per formati binari portabili.

## Esercizi

1. Definisci `typedef struct { ... } Rettangolo` con origine (Punto) e dimensioni. Scrivi `double area(const Rettangolo *r)`.
2. Definisci `typedef struct Nodo { int v; struct Nodo *next; } Nodo` e implementa una lista singolarmente concatenata con `lista_push_front`, `lista_stampa`, `lista_free`.
3. Crea una struct `Persona` con nome (array di `char` fisso), età (`int`), altezza (`double`). Popola un array di 3 persone con designated initializer.
4. Mostra che `sizeof` varia in funzione dell'ordine dei campi.
5. Scrivi un buffer con flexible array member e usa un singolo `malloc`.
