# Lezione 03 — Input e Output

In C l'I/O ("Input/Output", scrittura e lettura) è gestito interamente dalla libreria standard `<stdio.h>`. Il linguaggio in sé non sa nulla di tastiere o schermi: tutto passa per funzioni.

In questa lezione vediamo come **stampare** (output) e come **leggere** (input). Spoiler: l'output è facile, l'input è una **palude piena di insidie** — ed è proprio per questo che ci dedichiamo una lezione intera.

---

## 1. I tre stream standard

Quando un programma C parte, ha già **tre canali di comunicazione aperti** col mondo esterno:

| Stream   | Default       | Serve per                                  |
|----------|---------------|--------------------------------------------|
| `stdin`  | tastiera      | leggere input dall'utente                  |
| `stdout` | terminale     | output normale del programma               |
| `stderr` | terminale     | messaggi di errore e diagnostica           |

Sono variabili globali di tipo `FILE *` definite in `<stdio.h>`. Esistono **sempre**: non li apri, non li chiudi.

### Perché due stream di output separati?

Da shell puoi redirigerli **in modo indipendente**:

```bash
./programma                   # stdin = tastiera, stdout + stderr = terminale
./programma < input.txt       # stdin viene dal file
./programma > output.txt      # stdout va nel file, stderr resta sul terminale
./programma 2> errori.txt     # stderr va nel file, stdout resta sul terminale
./programma > out 2> err      # tutti e due nei file
./programma 2>&1 > all.txt    # stderr fuso dentro stdout
```

Quando metti il tuo programma in una **pipeline** (`./a | ./b | ./c`), passa di mano in mano solo `stdout`. Gli errori (`stderr`) escono direttamente all'utente, senza inquinare i dati. Per questo i messaggi diagnostici **vanno su `stderr`**, non su `stdout`.

---

## 2. `printf` — stampa formattata su stdout

```c
printf("formato", arg1, arg2, ...);
```

Il **formato** è una stringa che contiene testo normale e **segnaposti** che iniziano con `%`. Per ogni `%` deve esserci un argomento del tipo corretto.

### Specificatori principali

| Specificatore | Tipo           | Esempio                            |
|---------------|----------------|------------------------------------|
| `%d` `%i`     | `int`          | `printf("%d", 42)`                 |
| `%u`          | `unsigned int` | `printf("%u", 42u)`                |
| `%ld`         | `long`         | `printf("%ld", 42L)`               |
| `%lld`        | `long long`    | `printf("%lld", 42LL)`             |
| `%f`          | `double`       | `printf("%f", 3.14)`               |
| `%e`          | `double`, scientifico | `printf("%e", 1e6)` → `1.000000e+06` |
| `%c`          | `char`         | `printf("%c", 'A')`                |
| `%s`          | `char *` (stringa) | `printf("%s", "ciao")`         |
| `%p`          | puntatore      | `printf("%p", (void*)ptr)`         |
| `%x` `%X`     | esadecimale    | `printf("%x", 255)` → `ff`         |
| `%zu`         | `size_t`       | `printf("%zu", sizeof(int))`       |
| `%%`          | un `%` letterale | `printf("100%%")` → `100%`       |

### Modificatori (vanno tra `%` e la lettera)

```c
printf("%5d",   42);      // "   42"     larghezza minima 5
printf("%-5d",  42);      // "42   "     allineato a sinistra
printf("%05d",  42);      // "00042"     padding con zeri
printf("%.3f",  3.14159); // "3.142"     precisione: 3 decimali
printf("%8.2f", 3.14);    // "    3.14"  larghezza 8, precisione 2
```

### ⚠️ Trappola: tipo che non combacia

Se il tipo dell'argomento **non corrisponde** allo specificatore, è **Undefined Behavior** — stampi spazzatura o crashi:

```c
double x = 3.14;
printf("%d", x);           // 🐛 UB: %d vuole int, x e' double
printf("%f", 42);          // 🐛 UB: %f vuole double, 42 e' int
```

`gcc -Wall` ti avverte sempre quando i tipi non combaciano. **Leggi i warning**, sono il tuo guardiano.

---

## 3. `puts` e `putchar` — output minimale

Quando non ti serve formattare, niente di meglio di queste due:

```c
puts("ciao");        // stampa "ciao" + '\n' automaticamente
putchar('A');        // stampa un singolo carattere
```

`puts` aggiunge il newline da solo (e non interpreta i `%`, quindi `puts("100%")` è sicuro). `putchar` stampa **un byte** — utile dentro loop carattere per carattere.

---

## 4. `fprintf` — `printf` su uno stream qualsiasi

```c
fprintf(stream, "formato", arg1, arg2, ...);
```

Stesso formato di `printf`, ma scegli **dove** scrivere. L'uso più importante è scrivere su `stderr`:

```c
fprintf(stderr, "Errore: file non trovato\n");
```

> **Regola**: messaggi diagnostici / errori → `stderr`. Output "dati" → `stdout`. Non mischiarli, o renderai il programma inutilizzabile in pipeline.

(`printf(...)` è esattamente equivalente a `fprintf(stdout, ...)`.)

---

## 5. Il buffering — perché a volte i `printf` "spariscono"

`stdout` non scrive subito sul terminale: accumula i caratteri in un **buffer** interno e li scarica solo quando:

- si riempie il buffer, **oppure**
- viene scritto un `\n` (se `stdout` è collegato a un terminale: si chiama "line buffering"), **oppure**
- chiami `fflush(stdout)` esplicitamente, **oppure**
- il programma termina normalmente.

`stderr`, invece, è **unbuffered**: ogni byte va subito al destinatario.

### La trappola del prompt senza `\n`

```c
printf("Come ti chiami? ");      // 🐛 nessun \n, resta nel buffer
char nome[64];
fgets(nome, sizeof nome, stdin); // l'utente non vede ancora il prompt!
```

A schermo non comparirà nulla finché non scrivi un `\n` o non chiami `fflush`. Soluzioni:

```c
printf("Come ti chiami? ");
fflush(stdout);                  // forza la scrittura immediata
fgets(nome, sizeof nome, stdin);
```

### La trappola dell'ordine stdout/stderr

```c
printf("ciao");                  // line-buffered: aspetta un \n
fprintf(stderr, "errore\n");     // unbuffered: esce subito
```

Vedi prima "errore" e **poi** "ciao". Aggiungi `\n` ovunque (o `fflush`) e l'ordine torna naturale.

---

## 6. `scanf` — input formattato, **con prudenza**

```c
int x;
scanf("%d", &x);     // nota la & : passa l'INDIRIZZO di x, non il valore
```

`scanf` legge da `stdin` interpretando il formato. Sembra il duale di `printf`, ma è infinitamente più traditore. **In codice reale moderno `scanf` si evita** — vedremo a breve la combinazione `fgets` + `sscanf`/`strtol` che è la strada giusta. Qui impariamo `scanf` solo per capire **perché** ha problemi.

### Tre regole di sopravvivenza

**1. Controlla sempre il valore di ritorno.** `scanf` ritorna il **numero di campi convertiti con successo**. Se l'utente digita `pippo` invece di un numero, `scanf("%d", &n)` ritorna `0` e `n` resta non inizializzato.

```c
int n;
if (scanf("%d", &n) != 1) {
    fprintf(stderr, "input non valido\n");
    return 1;
}
```

**2. `%s` SENZA limite di lunghezza è una bomba di buffer overflow.** Se ne hai bisogno, specifica il limite:

```c
char buf[100];
scanf("%99s", buf);     // OK: leggi al massimo 99 char + il '\0' finale
scanf("%s", buf);       // 🚫 MAI: nessun limite, l'utente può scrivere 10MB
```

**3. `%d`, `%f`, `%s` ecc. saltano automaticamente lo whitespace iniziale** (spazi, tab, newline). `%c` invece **no**: legge il prossimo byte, anche se è uno spazio.

```c
char c;
scanf(" %c", &c);    // attento allo spazio prima di %c
//      ^ "salta lo whitespace prima di leggere il carattere"
```

### ⚠️ Il vero motivo per cui `scanf` fa male: lascia il `\n` in `stdin`

Quando l'utente digita `42⏎`, `scanf("%d", &n)` legge "42" e si ferma. Il `\n` **resta nel buffer di stdin**, pronto a sabotare la prossima lettura:

```c
int n;
scanf("%d", &n);                  // utente: "42⏎"  → n=42, stdin contiene ancora "\n"

char buf[64];
fgets(buf, sizeof buf, stdin);    // legge SUBITO quel '\n' → buf = "\n"
//                                  l'utente non puo' nemmeno digitare!
```

È il classico bug "perché il programma salta una domanda?". È risolvibile, ma con cerimonie noiose. Per questo passiamo subito al modo giusto.

---

## 7. `fgets` — il modo sicuro di leggere una riga

```c
char buf[128];
if (fgets(buf, sizeof buf, stdin) == NULL) {
    fprintf(stderr, "lettura fallita o EOF\n");
    return 1;
}
```

`fgets(buf, size, stream)`:
- legge **al massimo `size - 1`** caratteri (lo spazio per il `\0` finale è automatico);
- si ferma a **newline** (che **include** nel buffer) o a EOF;
- **non** fa mai overflow del buffer;
- ritorna `buf` su successo, **`NULL`** su errore o EOF immediato.

### ⚠️ `fgets` ritorna un **puntatore**, non un numero

Errore tipico del principiante:

```c
if (fgets(buf, sizeof buf, stdin) > 0) { ... }    // 🐛 confronto pointer/int
```

`fgets` ritorna `char *`. Il check corretto è solo:

```c
if (fgets(buf, sizeof buf, stdin) == NULL) { /* errore */ }
```

### Il `\n` resta NEL TUO buffer

`fgets` include il `\n` finale **nel buffer**, non lo butta. Esempio: l'utente digita `mawi⏎` →

```
indice:    0   1   2   3    4    5
buf:      'm' 'a' 'w' 'i' '\n' '\0'
```

Se vuoi una stringa "pulita" senza il `\n`, devi toglierlo tu:

```c
size_t len = strlen(buf);
if (len > 0 && buf[len-1] == '\n') {
    buf[len-1] = '\0';
}
```

Questo è il **rovescio della medaglia** rispetto a `scanf`: con `fgets`, il `\n` è dentro il **tuo** buffer (sotto il tuo controllo). Con `scanf`, il `\n` resta in `stdin` (fuori dal tuo controllo). **Fidati**: il primo è infinitamente più gestibile.

> Nota: se l'input è più lungo del buffer, `fgets` legge solo i primi `size-1` byte e **non** trova il `\n`. La riga viene "tagliata": il resto resta in stdin per la prossima `fgets`. Tieni il buffer abbondante.

---

## 8. Convertire una stringa in numero — `sscanf` e `strtol`

`fgets` ti dà una **stringa**. Per estrarne un numero ci sono due strade.

### `sscanf` — semplice, simile a `scanf` ma senza i problemi di stdin

```c
char buf[64];
fgets(buf, sizeof buf, stdin);

int eta;
if (sscanf(buf, "%d", &eta) != 1) {
    fprintf(stderr, "non e' un numero\n");
    return 1;
}
```

`sscanf` legge da una **stringa**, non da stdin: nessun effetto collaterale sullo stato di stdin. Stesso formato di `scanf`, con il solito check del valore di ritorno.

### `strtol` — più potente, controllo fine degli errori

Da `<stdlib.h>` + `<errno.h>` + `<limits.h>`:

```c
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

errno = 0;
char *end;
long n = strtol(buf, &end, 10);   // 10 = base decimale

if (end == buf)         { /* nessuna cifra trovata */ }
if (errno == ERANGE)    { /* overflow: numero troppo grande */ }
if (*end != '\0' && *end != '\n') { /* spazzatura dopo il numero */ }
```

`strtol` ti dice:
- **dove** ha smesso di leggere (`end`),
- se c'è stato un **overflow** (`errno == ERANGE`).

Varianti: `strtoll` (`long long`), `strtoul`/`strtoull` (unsigned), `strtod` (`double`).

Verboso? Sì. Robusto? Anche. È quello che usa il codice di produzione.

---

## 9. Il pattern raccomandato

Per **qualsiasi** input utente, usa **sempre** questo schema in due tempi:

```c
// 1) Leggi una RIGA in un buffer di char.
char buf[64];
if (fgets(buf, sizeof buf, stdin) == NULL) {
    fprintf(stderr, "lettura fallita\n");
    return 1;
}

// 2) Estrai dalla stringa quello che ti serve.
int eta;
if (sscanf(buf, "%d", &eta) != 1) {
    fprintf(stderr, "non era un numero\n");
    return 1;
}
```

Vantaggi:
- nessuno stato sporco in stdin,
- due check distinti (lettura vs parsing),
- funziona identico con file (`stdin` → `FILE *f`),
- puoi ri-parsare lo stesso buffer in più modi,
- puoi stampare `buf` per debug.

---

## 10. Errori frequenti

| Errore | Sintomo | Soluzione |
|--------|---------|-----------|
| Prompt senza `\n`, niente `fflush` | l'utente non vede il prompt | `fflush(stdout)` o aggiungi `\n` |
| Mischiare `scanf` e `fgets` | `fgets` legge "una riga vuota" | usa solo `fgets` + `sscanf`/`strtol` |
| `%s` senza limite di lunghezza | buffer overflow | `%99s` se il buffer è 100 |
| Specificatore non matchato | spazzatura o crash | leggi i warning `-Wall` |
| Confrontare `fgets(...)` con un numero | comparison error / sempre vero | confronto con `NULL` |
| Dimenticare di togliere il `\n` da `fgets` | output con riga in più | `if (buf[len-1]=='\n') buf[len-1]='\0';` |
| Output non in ordine | `stderr` esce prima di `stdout` | aggiungi `\n` o `fflush(stdout)` |

---

## Esercizi

1. Chiedi nome ed età all'utente e stampa un saluto personalizzato.
2. Leggi due numeri e stampa la loro somma, differenza, prodotto e quoziente (attenzione ai tipi).
3. Usa `fgets` per leggere una riga e stamparla al contrario.
4. Stampa un errore su `stderr` e un messaggio su `stdout` nello stesso programma; redirigi solo uno dei due a file.
