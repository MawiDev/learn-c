# Lezione 09 — Stringhe

In C **non esiste un tipo "stringa"**. Una stringa è:

> un **array di `char`** terminato dal byte nullo `'\0'`.

```c
char s[] = "Ciao";
```

In memoria:
```
 'C' 'i' 'a' 'o' '\0'
  0   1   2   3   4
```

Il `'\0'` (byte 0) è il **terminatore**. Senza di lui, le funzioni della libreria standard non sanno dove finisce la stringa → accessi out-of-bounds → disastri.

## Stringa letterale

```c
char *s1 = "Ciao";     // s1 punta a memoria READ-ONLY
char  s2[] = "Ciao";   // s2 è un array modificabile, copia del letterale
```

- `s1[0] = 'X';` → **UB**, tipicamente segfault.
- `s2[0] = 'X';` → OK.

Nel codice moderno preferiamo: `const char *s = "Ciao";` per rendere esplicita l'immutabilità.

## `<string.h>` — le funzioni classiche

```c
#include <string.h>

size_t strlen(const char *s);                          /* lunghezza, esclude \0 */
char  *strcpy(char *dst, const char *src);             /* copia, PERICOLOSA */
char  *strncpy(char *dst, const char *src, size_t n);  /* limite, ma non garantisce \0 */
char  *strcat(char *dst, const char *src);             /* concatena, PERICOLOSA */
int    strcmp(const char *a, const char *b);           /* <0, 0, >0 */
int    strncmp(const char *a, const char *b, size_t n);
char  *strchr(const char *s, int c);                   /* trova carattere */
char  *strstr(const char *hay, const char *needle);    /* trova sottostringa */
void  *memcpy(void *dst, const void *src, size_t n);
void  *memset(void *dst, int c, size_t n);
int    memcmp(const void *a, const void *b, size_t n);
```

### Perché `strcpy` è pericolosa

```c
char buf[8];
strcpy(buf, "questa-e-molto-lunga");   // overflow!
```

Non c'è controllo: se `src` è più lunga del buffer di destinazione, corrompi memoria. Buffer overflow = vulnerabilità di sicurezza.

### Regola del C moderno

1. **Mai `strcpy` / `strcat` / `gets`**. Vietati nel codice reale.
2. Preferisci `snprintf` per costruire stringhe:
   ```c
   char buf[64];
   int scritti = snprintf(buf, sizeof buf, "Ciao, %s! Hai %d anni.", nome, eta);
   if (scritti < 0 || (size_t)scritti >= sizeof buf) {
       // troncamento o errore
   }
   ```
   `snprintf` **non** fa overflow e **sempre** termina con `'\0'` (se `size > 0`).

3. `strncpy` NON è "la versione sicura di `strcpy`": se `src` è lunga `>= n` non aggiunge `'\0'`. Usa `snprintf` o scrivi la tua copia sicura.

## Esempi idiomatici

### Lunghezza manuale (educational)

```c
size_t mia_strlen(const char *s) {
    const char *p = s;
    while (*p) ++p;
    return (size_t)(p - s);
}
```

### Confronto

```c
if (strcmp(a, b) == 0) { /* uguali */ }
```

### Scansione carattere per carattere

```c
for (const char *p = s; *p; ++p) {
    if (*p == ' ') ...
}
```

## Array di stringhe

```c
const char *giorni[] = {
    "Lun", "Mar", "Mer", "Gio", "Ven", "Sab", "Dom"
};
printf("%s\n", giorni[0]);
```

È un array di puntatori a stringhe letterali (read-only).

## `char` vs `unsigned char` e funzioni `<ctype.h>`

```c
#include <ctype.h>

isalpha(c)   /* è una lettera? */
isdigit(c)   /* è una cifra? */
isspace(c)   /* spazio, tab, newline... */
toupper(c)   /* converte a maiuscolo */
tolower(c)
```

**Attenzione**: queste funzioni richiedono che `c` sia un `unsigned char` convertito a `int`, oppure `EOF`. Con `char` signed e caratteri > 127 hai UB:

```c
for (const char *p = s; *p; ++p) {
    if (isdigit((unsigned char)*p)) { ... }   // cast necessario
}
```

## UTF-8 — una nota pragmatica

Il C "di base" lavora su byte, non su caratteri Unicode. Una stringa UTF-8 in `char[]` funziona per stoccaggio e stampa, ma `strlen` conta byte, non caratteri. Per UTF-8 reale servono librerie (ICU, utf8.h) o C11 `<uchar.h>` (`char16_t`, `char32_t`). Fuori scope per ora — sappi solo che `strlen("è") != 1`.

## Esercizi

1. Implementa `size_t mia_strlen(const char *s)`.
2. Scrivi `void a_maiuscolo(char *s)` che converte in-place (usa `<ctype.h>` con cast).
3. Conta le vocali in una stringa.
4. Scrivi `bool palindroma(const char *s)` ignorando maiuscole/minuscole.
5. Dato un buffer di dimensione 64, usa `snprintf` per costruire una stringa tipo `"ID=%04d, nome=%s"` senza mai fare overflow, e gestisci il troncamento.
