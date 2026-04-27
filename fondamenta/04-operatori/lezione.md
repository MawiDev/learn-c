# Lezione 04 — Operatori

## Aritmetici

| Op | Significato | Esempio |
|----|-------------|---------|
| `+` | somma       | `3 + 2` → `5` |
| `-` | differenza  | `3 - 2` → `1` |
| `*` | prodotto    | `3 * 2` → `6` |
| `/` | quoziente   | `7 / 2` → `3` (int!) — `7.0 / 2` → `3.5` |
| `%` | resto       | `7 % 2` → `1` (solo su interi) |

Incremento e decremento:

```c
int i = 5;
i++;    // postfisso: ritorna 5, POI incrementa
++i;    // prefisso:  incrementa PRIMA, ritorna 6
```

Nel C moderno, quando non serve il valore dell'espressione, il risultato è identico. Scegli quello che si legge meglio (tipicamente `++i`).

**Non scrivere codice come `a[i] = i++;`**: il valore è UB. Una sola modifica di `i` per espressione.

## Assegnazione composta

```c
x += 3;    // x = x + 3
x -= 3;
x *= 2;
x /= 2;
x %= 2;
x <<= 1;   // shift a sinistra
x >>= 1;
x &= m;
x |= m;
x ^= m;
```

## Relazionali e logici

| Op  | Significato |
|-----|-------------|
| `==` | uguale |
| `!=` | diverso |
| `<`  `>` `<=` `>=` | confronti |
| `&&` | AND logico (short-circuit) |
| `||` | OR logico (short-circuit) |
| `!`  | NOT logico |

In C **il risultato di un operatore logico è `int`** (0 o 1), non `bool`. Funziona lo stesso, ma sappilo.

**Short-circuit**: `a && b` non valuta `b` se `a` è falso. Questo è utile:

```c
if (p != NULL && p->valore > 0) { ... }  // accesso SICURO
```

Scritto al contrario esploderebbe in caso di `p == NULL`.

### L'errore più comune in C

```c
if (x = 5) { ... }     // SBAGLIATO: assegnazione, sempre vera
if (x == 5) { ... }    // GIUSTO: confronto
```

Il compilatore con `-Wall` ti avverte. Alcuni scrivono `if (5 == x)` (stile Yoda) per prevenire: è una questione di gusti, io preferisco leggere il warning.

## Bitwise (sui bit)

| Op | Significato |
|----|-------------|
| `&` | AND bit-a-bit |
| `|` | OR bit-a-bit |
| `^` | XOR bit-a-bit |
| `~` | NOT bit-a-bit |
| `<<` | shift a sinistra |
| `>>` | shift a destra |

Idiomi fondamentali:

```c
uint32_t flag = 1u << 3;     // bit 3 settato:    0b1000
x |= flag;                   // set bit
x &= ~flag;                  // clear bit
x ^= flag;                   // toggle bit
if (x & flag) { ... }        // test bit
```

**Importante**: lo shift a sinistra di un numero **signed** che ne altera il bit di segno è **UB**. Fai bitwise su `unsigned` (es. `uint32_t`) quando possibile.

## Operatore ternario

```c
int max = (a > b) ? a : b;
```

Conciso e ottimo per valori. Non abusarne nel controllo di flusso complesso: diventa illeggibile.

## `sizeof`

```c
sizeof(int)       // tipo
sizeof x          // espressione (parentesi opzionali)
```

Restituisce un `size_t` a tempo di compilazione (salvo VLA). È un **operatore**, non una funzione.

## Operatore virgola `,`

```c
int a = (x = 1, y = 2, x + y);   // a = 3
```

Valuta da sinistra a destra e restituisce l'ultimo valore. Lo vedrai nei `for`. Raro altrove.

## Precedenza (estratto pratico)

Dalla più alta alla più bassa (semplificato):

1. `()` `[]` `->` `.` `++` `--` (postfissi)
2. `!` `~` `++` `--` `+` `-` (unari), `*` `&` (dereferenza/address-of), `sizeof`, cast
3. `*` `/` `%`
4. `+` `-`
5. `<<` `>>`
6. `<` `<=` `>` `>=`
7. `==` `!=`
8. `&`
9. `^`
10. `|`
11. `&&`
12. `||`
13. `?:`
14. `=` `+=` `-=` ...
15. `,`

**Regola pratica**: quando il dubbio sfiora, metti le parentesi. Costano zero e rendono il codice ovvio.

```c
if ((a & MASK) == EXPECTED) { ... }   // esplicito
```

## Esercizi

1. Scrivi un programma che calcola media aritmetica e geometrica di due `double`.
2. Verifica se un `int` è pari usando `&` invece di `%`.
3. Data una `uint8_t`, stampa i suoi 8 bit (usa `>>` e `&`).
4. Scrivi `max(a, b)` senza usare `if`, solo con operatore ternario.
5. Qual è il valore di `1 + 2 * 3 << 1`? Calcolalo **a mente** e poi verifica.
