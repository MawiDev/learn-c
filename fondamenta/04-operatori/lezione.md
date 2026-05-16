# Lezione 04 — Operatori

Un **operatore** è un simbolo che combina uno, due o tre valori (gli "operandi") e produce un nuovo valore. Già conosci `+` dalla matematica: in C funziona come ti aspetti, ma C ne ha molti altri e con qualche trappola. Vediamoli tutti.

---

## 1. Operatori aritmetici

| Op | Nome | Esempio | Risultato |
|----|------|---------|-----------|
| `+` | somma | `3 + 2` | `5` |
| `-` | differenza | `3 - 2` | `1` |
| `*` | prodotto | `3 * 2` | `6` |
| `/` | quoziente | `7 / 2` | **`3`** (interi!) |
| `/` | quoziente | `7.0 / 2` | `3.5` |
| `%` | resto | `7 % 2` | `1` |

### La trappola della divisione

Questa è la sorpresa numero uno per chi viene da Python o da JavaScript:

```c
int a = 7;
int b = 2;
printf("%d\n", a / b);    // stampa 3, non 3.5
```

**Regola**: se entrambi gli operandi di `/` sono interi, C fa la **divisione intera** (tronca verso lo zero). Per avere il risultato in virgola mobile, almeno **uno** dei due operandi deve essere `double` (o `float`):

```c
printf("%.1f\n", 7.0 / 2);       // 3.5
printf("%.1f\n", (double)a / b); // 3.5  — cast esplicito
printf("%.1f\n", a / 2.0);       // 3.5
```

**Attenzione all'ordine del cast**: `(double)(a / b)` calcola PRIMA la divisione intera (= 3), poi la converte in `double` (= 3.0). Inutile. Casta UN operando, non il risultato.

### Il modulo (`%`)

`%` è il **resto della divisione intera**: `7 % 2` è `1` perché `7 = 3*2 + 1`.

```c
17 % 5    // 2
20 % 4    // 0  (20 è divisibile per 4)
-7 % 2    // -1 (il segno segue il dividendo, da C99)
```

**Solo su interi**: `7.5 % 2` non compila. Per il modulo su `double` esiste `fmod` in `<math.h>`, ma è un'altra storia.

Usi tipici: testare divisibilità (`n % 2 == 0` → pari), "avvolgere" un indice (`i % LENGTH`).

### Incremento e decremento

```c
int i = 5;
i++;    // postfisso: "il valore è 5, poi diventa 6"
++i;    // prefisso:  "diventa 6, e il valore è 6"
```

Detto in altre parole, entrambi modificano `i`, ma **come espressioni** hanno valori diversi:

```c
int i = 5;
int a = i++;    // a = 5 (vecchio valore di i), poi i diventa 6
int j = 5;
int b = ++j;    // j diventa 6, poi b = 6 (nuovo valore)
```

Quando il valore dell'espressione **non serve** (es. dentro un `for`), `i++` e `++i` sono **identici**. Scegli quello più leggibile — la convenzione moderna è `++i`.

> **REGOLA D'ORO**: **una sola modifica** della stessa variabile per espressione. Roba tipo `a[i] = i++;` o `f(i++, i++)` è **Undefined Behavior** — il C non garantisce in che ordine vengono valutate le sotto-espressioni. Non scriverlo. Mai.

---

## 2. Assegnazione composta

Scorciatoie per "modifica X usando il suo valore corrente":

```c
x += 3;    // equivale a:  x = x + 3
x -= 3;    //              x = x - 3
x *= 2;    //              x = x * 2
x /= 2;    //              x = x / 2
x %= 2;    //              x = x % 2
x <<= 1;   //              x = x << 1   (shift, vedi bitwise)
x >>= 1;
x &= m;    //              x = x & m
x |= m;
x ^= m;
```

Non sono solo zucchero sintattico: se a sinistra c'è un'espressione complessa, scrivendo `arr[i+j].campo += 1` non valuti due volte `i+j` e l'indicizzazione, come faresti con `arr[i+j].campo = arr[i+j].campo + 1`. Più conciso, più sicuro, spesso più veloce.

---

## 3. Operatori relazionali e logici

### Relazionali (confronti)

| Op | Significato | Esempio |
|----|-------------|---------|
| `==` | uguale | `x == 5` |
| `!=` | diverso | `x != 0` |
| `<` `>` `<=` `>=` | confronti | `x < 100` |

### Logici (combinano condizioni)

| Op | Significato |
|----|-------------|
| `&&` | AND — vero se **entrambi** sono veri |
| `||` | OR — vero se **almeno uno** è vero |
| `!` | NOT — inverte vero/falso |

> **In C il "vero" è un `int` con valore 1, il "falso" è 0.** Tutti gli operatori logici restituiscono `int`, non `bool`. Funziona uguale, ma sappilo perché ogni tanto serve.

### Short-circuit: la valutazione "pigra"

`&&` e `||` valutano gli operandi **da sinistra a destra** e si fermano appena il risultato è certo. È un comportamento garantito dallo standard, non un'ottimizzazione.

```c
if (a > 0 && b/a > 10) { ... }
//   ^^^^^   ^^^^^^^^
//   1°       2° — calcolato SOLO se a > 0
```

Se `a == 0`, la seconda parte non viene valutata: niente divisione per zero. Senza short-circuit, esploderebbe.

L'idioma più importante in cui lo userai è il check di puntatore prima di dereferenziare:

```c
if (p != NULL && p->valore > 0) { ... }   // SICURO
if (p->valore > 0 && p != NULL) { ... }   // CRASH se p == NULL
```

L'ordine conta. Tieni a mente questa regola: **prima il check di esistenza, poi l'uso**.

### `=` vs `==` — l'errore più frequente in C

```c
if (x = 5) { ... }     // ❌ ASSEGNAZIONE: assegna 5 a x. L'if e' sempre vero.
if (x == 5) { ... }    // ✅ CONFRONTO
```

In C `=` è l'assegnazione, `==` è il confronto. Sembra ovvio detto così, ma fidati: prima o poi lo sbagli. Buona notizia: `gcc -Wall` ti avverte sempre quando vede un'assegnazione dentro un `if`. Leggi i warning.

(Esiste lo "stile Yoda": scrivere `if (5 == x)` per far errore se sbagli — l'inverso, `if (5 = x)`, non compila. È un gusto.)

---

## 4. Operatori bitwise (sui singoli bit)

Lavorano sui bit interi del numero. Servono per: flag, maschere, protocolli di rete, formati binari, ottimizzazioni.

| Op | Nome | Cosa fa |
|----|------|---------|
| `&` | AND | bit per bit: `1 & 1 = 1`, altrimenti `0` |
| `\|` | OR | bit per bit: `0 \| 0 = 0`, altrimenti `1` |
| `^` | XOR | bit per bit: `1` se diversi, `0` se uguali |
| `~` | NOT | inverte ogni bit |
| `<<` | shift sinistro | sposta i bit a sinistra (= moltiplica per 2) |
| `>>` | shift destro | sposta i bit a destra (= divide per 2) |

### Vediamolo coi bit

`uint8_t a = 0b10110100` (= 180 in decimale), `uint8_t b = 0b11010110` (= 214):

```
   a    1 0 1 1 0 1 0 0
   b    1 1 0 1 0 1 1 0
   ─────────────────────
 a & b  1 0 0 1 0 1 0 0   ← AND
 a | b  1 1 1 1 0 1 1 0   ← OR
 a ^ b  0 1 1 0 0 0 1 0   ← XOR
   ~a   0 1 0 0 1 0 1 1   ← NOT
```

Shift di `0b00001101` (= 13):

```
       13      0 0 0 0 1 1 0 1
   13 << 2     0 0 1 1 0 1 0 0    = 52   (= 13 * 4)
   13 >> 1     0 0 0 0 0 1 1 0    = 6    (= 13 / 2)
```

Lo shift sinistro di `n` posizioni equivale a moltiplicare per `2^n`; lo shift destro a dividere. Molto più veloce di `*` e `/`, e in alcuni casi il compilatore lo fa per te.

### I 4 idiomi che incontrerai sempre

Dato un `uint32_t x` e una maschera `flag = 1u << 3` (che è `0b1000`, cioè "il bit 3 acceso"):

```c
x |= flag;        // SET bit 3:    accendi il bit, lascia gli altri
x &= ~flag;       // CLEAR bit 3:  spegni il bit, lascia gli altri
x ^= flag;        // TOGGLE bit 3: inverti il bit, lascia gli altri
if (x & flag)     // TEST bit 3:   vero se il bit e' acceso
```

Imparali a memoria. Sono ovunque nel kernel, nei driver, nei file binari.

### Trappola: shift su signed

Lo shift sinistro di un numero **signed** che ne altera il bit di segno è **Undefined Behavior**:

```c
int x = 0x40000000;
x = x << 1;    // UB: il bit di segno cambia
```

**Soluzione**: per la manipolazione di bit usa sempre tipi **unsigned** — `uint8_t`, `uint16_t`, `uint32_t`, `uint64_t`. Non solo evita l'UB: rende esplicito al lettore che stai facendo bit-twiddling.

---

## 5. Operatore ternario `? :`

L'unico operatore con tre operandi. Forma:

```c
condizione ? valore_se_vero : valore_se_falso
```

Eccezionalmente utile per **espressioni** (non per controllo di flusso). Sostituisce un mini-`if` quando devi solo scegliere un valore:

```c
int max = (a > b) ? a : b;
const char *msg = errore ? "fallito" : "ok";
printf("hai %d %s\n", n, n == 1 ? "anno" : "anni");
```

**Non abusarne**: ternari annidati diventano illeggibili in fretta.

```c
// Leggibile:
int sign = (x > 0) ? 1 : (x < 0) ? -1 : 0;

// Illeggibile, riscrivilo come if/else:
int v = a ? b ? c ? d : e : f : g;    // 🚫
```

Regola: se non lo capisci a colpo d'occhio, riscrivi con `if/else`.

---

## 6. `sizeof` — è un **operatore**, non una funzione

Restituisce la **dimensione in byte** di un tipo o di un'espressione. Si valuta a **tempo di compilazione** (salvo VLA, vedi lezione 08):

```c
sizeof(int)       // dimensione del tipo int       — parentesi obbligatorie
sizeof x          // dimensione della variabile x  — parentesi opzionali
sizeof(x + 1)     // ok, ma x + 1 NON viene eseguito: solo il tipo conta
```

Il risultato è un `size_t` (intero unsigned). Per stamparlo:

```c
printf("sizeof(int) = %zu\n", sizeof(int));   // %zu è il formato per size_t
```

> **Non è una funzione**: nessuna chiamata, nessun overhead. Per il compilatore `sizeof(int)` è una costante come `4`.

Usi più comuni:
- Argomento di `malloc` (lezione su memoria dinamica): `malloc(n * sizeof(int))`.
- Dimensione di un buffer per `fgets`/`snprintf`: `fgets(buf, sizeof buf, stdin)`.
- Calcolo numero elementi di un array: `sizeof arr / sizeof arr[0]` (lezione 08).

---

## 7. Operatore virgola `,`

Valuta più espressioni in sequenza da sinistra a destra, e il valore complessivo è quello dell'**ultima**:

```c
int a = (x = 1, y = 2, x + y);   // x=1, y=2, a=3
```

In pratica lo vedrai **solo** nel `for` per gestire più contatori:

```c
for (int i = 0, j = 10; i < j; ++i, --j) { ... }
//                                ^^^^^
//                                qui è l'operatore virgola
```

Fuori dal `for`, evitalo: confonde e basta. (Attenzione: la **virgola** tra parametri di funzione o tra dichiarazioni di variabile **non è** questo operatore — è un separatore sintattico.)

---

## 8. Precedenza degli operatori

Quando in un'espressione ci sono più operatori senza parentesi, C usa una **tabella di precedenza** per decidere chi viene prima. Esempio: `1 + 2 * 3` fa `7`, non `9`, perché `*` ha precedenza più alta di `+`.

### Le 5 regole pratiche da ricordare

Memorizzare la tabella completa non serve. Bastano queste:

1. **Aritmetici prima dei confronti**: `a + 1 < n` è `(a + 1) < n`.
2. **Confronti prima dei logici**: `x > 0 && x < 10` è `(x > 0) && (x < 10)`.
3. **`*` `/` `%` prima di `+` `-`** (come a scuola).
4. **`&&` prima di `||`**: `a || b && c` è `a || (b && c)`.
5. **Bitwise hanno precedenza BIZZARRA**: `x & MASK == VAL` significa `x & (MASK == VAL)` — quasi mai quello che volevi! **Metti sempre le parentesi attorno alle espressioni bitwise nei confronti**:
   ```c
   if ((x & MASK) == VAL) { ... }   // ✅
   if (x & MASK == VAL) { ... }     // 🐛 quasi sempre sbagliato
   ```

### Tabella completa (riferimento)

Dalla precedenza più **alta** alla più **bassa**:

| Liv. | Operatori |
|------|-----------|
| 1 (max) | `()` `[]` `->` `.` (postfix), `i++` `i--` |
| 2 | `!` `~` `++i` `--i` `+x` `-x` (unari), `*p` `&x`, `sizeof`, `(tipo)` cast |
| 3 | `*` `/` `%` |
| 4 | `+` `-` |
| 5 | `<<` `>>` |
| 6 | `<` `<=` `>` `>=` |
| 7 | `==` `!=` |
| 8 | `&` (bitwise) |
| 9 | `^` |
| 10 | `\|` |
| 11 | `&&` |
| 12 | `\|\|` |
| 13 | `? :` |
| 14 | `=` `+=` `-=` ... |
| 15 (min) | `,` |

### La regola d'oro

> **Nel dubbio, parentesi.** Costano zero, valgono mille.

```c
int v = (a + b) * (c - d);          // chiarissimo
if ((flag & MASK) == EXPECTED) { }  // niente sorprese
```

Il codice non si scrive per il compilatore, si scrive per il prossimo umano che lo legge. Spesso quel prossimo umano sei tu, tra sei mesi.

---

## Esercizi

1. Scrivi un programma che calcola media aritmetica e geometrica di due `double`.
2. Verifica se un `int` è pari usando `&` invece di `%`.
3. Data una `uint8_t`, stampa i suoi 8 bit (usa `>>` e `&`).
4. Scrivi `max(a, b)` senza usare `if`, solo con operatore ternario.
5. Qual è il valore di `1 + 2 * 3 << 1`? Calcolalo **a mente** e poi verifica.
