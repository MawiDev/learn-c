# Lezione 09 (medi) — Tipi moderni (C11 / C17 / C23)

Questa è la lezione che ti distingue da chi scrive ancora C "stile K&R del 1978". Il C moderno ti regala strumenti utili: usali.

## `<stdint.h>` — dimensioni esatte

```c
#include <stdint.h>

int8_t    i8;      // esatto
int16_t   i16;
int32_t   i32;
int64_t   i64;

uint8_t   u8;      // senza segno, esatto
uint16_t  u16;
uint32_t  u32;
uint64_t  u64;

intptr_t  ip;      // intero abbastanza grande per un puntatore
uintptr_t up;
size_t    n;       // dimensioni (unsigned)
ptrdiff_t d;       // differenza tra puntatori (signed)
```

Vedi anche:
- `INT32_MAX`, `UINT64_MAX` — limiti esatti.
- `<inttypes.h>` → formatters `PRId32`, `PRIu64`, eccetera:
  ```c
  uint64_t n = 42;
  printf("n = %" PRIu64 "\n", n);
  ```

Quando scrivi protocolli di rete o formati binari, **solo** tipi a dimensione esatta. Mai `int`.

## `<stdbool.h>` (C99+)

```c
#include <stdbool.h>

bool ok = true;
if (ok) { ... }
```

In C23: `bool`, `true`, `false` sono **keyword**, `<stdbool.h>` non serve più.

## `<stddef.h>`

Definizioni fondamentali:
- `size_t` → unsigned, per dimensioni.
- `ptrdiff_t` → signed, per differenze di puntatori.
- `NULL` → puntatore nullo.
- `offsetof(tipo, membro)` → offset di un campo in una struct.

## `_Static_assert` (C11)

Asserzione a **tempo di compilazione**:

```c
_Static_assert(sizeof(int) >= 4, "int deve essere >= 32 bit");
_Static_assert(sizeof(void*) == 8, "solo 64-bit supportato");
```

In C23: scritto `static_assert`, e la string può essere omessa.

Usalo per:
- verifiche di ABI (dimensioni struct),
- assunzioni piattaforma,
- constraint su valori enum.

## `_Generic` (C11) — "overloading" a tempo di compilazione

```c
#define tipo_di(x) _Generic((x), \
    int:    "int",                 \
    double: "double",              \
    char *: "char *",              \
    default:"sconosciuto")

printf("%s\n", tipo_di(3));          // "int"
printf("%s\n", tipo_di(3.14));       // "double"
```

`_Generic` seleziona un'espressione in base al **tipo** del primo argomento. Abilita macro type-safe:

```c
#define ABS(x) _Generic((x), \
    int:    abs,              \
    long:   labs,             \
    float:  fabsf,            \
    double: fabs)(x)
```

Non è un vero sistema di template, ma risolve molti casi.

## Thread e atomic (C11)

```c
#include <threads.h>     // thread portabili
#include <stdatomic.h>   // operazioni atomiche

atomic_int counter = 0;
atomic_fetch_add(&counter, 1);
```

`<threads.h>` non è supportato ovunque (su molti sistemi si usa direttamente `pthread`). `<stdatomic.h>` è più diffuso.

Ci torneremo in un corso avanzato. Sappi che esistono.

## Novità C23 (rapide)

- `true`, `false`, `bool`, `nullptr` come keyword.
- Attributi standardizzati: `[[nodiscard]]`, `[[deprecated]]`, `[[fallthrough]]`, `[[maybe_unused]]`, `[[noreturn]]`.
- `typeof(x)` — ora standard (era GCC).
- `constexpr` per oggetti.
- Letterali binari: `0b1010`.
- Digit separators: `1'000'000`.
- `#embed` per embeddare file binari nel sorgente.
- `unreachable()` come intrinseco standard.

Il supporto nei compilatori migliora mese per mese. Con GCC ≥ 13 / Clang ≥ 16 puoi compilare con `-std=c2x` o `-std=c23` per provarle.

Esempio C23:

```c
[[nodiscard]] int parse_int(const char *s);
[[maybe_unused]] int debug_x = 42;

int arr[] = { 0b1010, 0b1100, 100'000 };
```

## `noreturn` (C11)

```c
#include <stdnoreturn.h>
noreturn void fatal(const char *msg);
```

Indica al compilatore che la funzione non ritorna mai (tipo `exit`, `abort`, `longjmp`). Abilita ottimizzazioni e silenzia warning. In C23 è `[[noreturn]]`.

## Inline (C99+)

```c
static inline int clamp(int x, int lo, int hi) {
    return x < lo ? lo : (x > hi ? hi : x);
}
```

`static inline` in un header è il modo pulito di scrivere piccole funzioni helper senza problemi di "multiple definition". Meglio delle macro per il 90% dei casi.

## `<time.h>` moderno

```c
struct timespec ts;
clock_gettime(CLOCK_MONOTONIC, &ts);
```

Per benchmark e misurazioni preferisci `CLOCK_MONOTONIC` (non salta per regolazioni orario) a `time()` o `clock()`.

C11 aggiunge `timespec_get` come variante portabile:
```c
struct timespec ts;
timespec_get(&ts, TIME_UTC);
```

## Esercizi

1. Riscrivi una funzione che accetta `int` e fallo accettare un `int32_t`. Stampa con `PRId32`.
2. Usa `_Static_assert` per verificare che una struct che rappresenta un pacchetto di rete sia esattamente di 16 byte.
3. Scrivi una macro `PRINT(x)` che usa `_Generic` per stampare `int`, `double`, `char *` con il formato giusto.
4. Misura il tempo di un loop con `clock_gettime(CLOCK_MONOTONIC, ...)`.
5. Prova a compilare con `-std=c2x` un programma che usa `nullptr` e `[[nodiscard]]`.
