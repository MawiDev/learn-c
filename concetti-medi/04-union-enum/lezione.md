# Lezione 04 (medi) — `union` ed `enum`

## `enum` — costanti intere con nome

```c
enum Colore { ROSSO, VERDE, BLU };     // 0, 1, 2

enum HTTP { OK = 200, NOT_FOUND = 404, SERVER_ERROR = 500 };
```

Valori assegnati automaticamente partendo da 0, oppure espliciti.

Pattern tipico con `typedef`:

```c
typedef enum {
    STATO_IDLE,
    STATO_RUN,
    STATO_STOP
} Stato;

Stato s = STATO_RUN;
```

### Cosa è *realmente* un enum

È un `int` sotto il cofano (più o meno). In C23 diventa un tipo con dimensione **specificabile**:

```c
enum Piccolo : uint8_t { ... };   // C23
```

Finché non usi C23, non assumere che `enum` sia `int`: su alcune piattaforme è più piccolo.

### Perché usare enum invece di `#define`

1. Il debugger ti mostra `STATO_RUN` invece di `1`.
2. Il compilatore sa che è un tipo (può avvisarti su `switch` incompleti con `-Wswitch`).
3. Ha scope (in C23 puoi anche avere enum non esportati).

## `union` — stessa memoria, interpretazioni diverse

```c
union Valore {
    int    i;
    float  f;
    char   s[8];
};

union Valore v;
v.i = 0x41424344;
printf("come stringa: %s\n", v.s);
```

Tutti i membri condividono la stessa memoria. `sizeof(union V)` è la dimensione del membro più grande.

**Regola del C**: leggere un membro diverso da quello che hai appena scritto è **implementation-defined** (con un'eccezione per `unsigned char[]` per ispezione byte). Per type-punning portabile si usa `memcpy`.

## Il pattern "tagged union" (tipi somma manuali)

```c
typedef enum { T_INT, T_DOUBLE, T_STRING } Tipo;

typedef struct {
    Tipo tipo;
    union {
        int         i;
        double      d;
        const char *s;
    } v;
} Valore;

void stampa(const Valore *val) {
    switch (val->tipo) {
        case T_INT:    printf("%d\n", val->v.i); break;
        case T_DOUBLE: printf("%g\n", val->v.d); break;
        case T_STRING: printf("%s\n", val->v.s); break;
    }
}
```

Questo è come si implementa un "tipo variante" in C. Sempre associa l'union a un **tag** (`enum`) che dice "quale membro è attualmente valido".

### Anonymous union / struct (C11+)

```c
typedef struct {
    Tipo tipo;
    union {                 // anonima → non serve nominarla
        int         i;
        double      d;
        const char *s;
    };                      // non .v davanti
} Valore;

val.i = 42;     // accesso diretto, pulito
```

Abilita codice molto più leggibile. Usa questa forma ovunque possibile.

## Quando usare `union`

1. **Tagged union** (sopra).
2. **Hardware registers** (i bit di un registro interpretati come int o bitfield).
3. **Memoria risparmiata** in strutture dati dove i campi sono mutuamente esclusivi.

Raramente la userai. Quando la userai, lo saprai.

## Esercizi

1. Definisci `enum Giorno` con LUN..DOM e scrivi una funzione che dato un `Giorno` ritorna la stringa del nome.
2. Scrivi un `switch` esaustivo su un `enum` e togli un caso: osserva il warning `-Wswitch`.
3. Implementa il tipo `Valore` (tagged union) con i tipi INT, DOUBLE, STRING. Scrivi `stampa` e `uguali(const Valore *a, const Valore *b)`.
4. Dimostra con `sizeof` la differenza tra una struct con tutti i campi e una union degli stessi campi.
5. Usa `memcpy` per fare type-punning tra `uint32_t` e `float` (simula `*(float*)&u`).
