# Lezione 05 (medi) — File I/O

Il C lavora con i file tramite `FILE *`, un handle opaco definito in `<stdio.h>`.

## Apertura

```c
FILE *f = fopen("file.txt", "r");
if (f == NULL) {
    perror("fopen");
    return 1;
}
```

Modi più usati:

| Modo   | Significato |
|--------|-------------|
| `"r"`  | lettura. Il file deve esistere. |
| `"w"`  | scrittura. **Tronca** il file (o lo crea). |
| `"a"`  | append. Crea se non esiste. |
| `"r+"` | lettura + scrittura, dall'inizio. |
| `"w+"` | lettura + scrittura, tronca. |
| `"a+"` | lettura + append. |

Aggiungi `"b"` per modalità **binaria**: `"rb"`, `"wb"`. Su Unix cambia poco; su Windows è **cruciale** (niente conversione `\n` ↔ `\r\n`).

## Chiusura

```c
fclose(f);
```

**Sempre**. `fclose` flusha i buffer e libera risorse. Controlla il valore di ritorno (0 = OK, EOF = errore): è comune che i write pendenti emergano solo al close.

## Lettura testuale

### Riga per riga con `fgets`

```c
char buf[256];
while (fgets(buf, sizeof buf, f) != NULL) {
    /* buf termina in '\n' se c'era spazio */
    fputs(buf, stdout);
}
```

### Carattere per carattere

```c
int c;
while ((c = fgetc(f)) != EOF) {
    putchar(c);
}
```

Nota: `fgetc` ritorna `int`, non `char`, perché deve poter rappresentare `EOF` (tipicamente `-1`). Non scrivere mai `char c = fgetc(f);` per poi confrontarlo con `EOF`: con `char` unsigned non funziona.

### Parsing con `fscanf` (con cautela)

```c
int a, b;
if (fscanf(f, "%d %d", &a, &b) == 2) {
    ...
}
```

Stesse insidie di `scanf`. Preferisci `fgets` + parsing manuale o `sscanf`.

## Scrittura testuale

```c
fprintf(f, "valore = %d\n", 42);
fputs("una riga\n", f);
fputc('A', f);
```

## I/O binario

Per dati strutturati **non testuali** (numeri, struct, immagini):

```c
size_t fread (void *dst, size_t elem_size, size_t n, FILE *f);
size_t fwrite(const void *src, size_t elem_size, size_t n, FILE *f);
```

Ritornano il numero di **elementi** letti/scritti (non byte).

```c
double buf[100];
size_t n = fread(buf, sizeof buf[0], 100, f);
if (n < 100 && ferror(f)) {
    perror("fread");
}
```

**Attenzione al padding delle struct**: non serializzare una struct con `fwrite(&s, sizeof s, 1, f)` se aspetti portabilità. Scrivi campo per campo in un formato definito.

## Posizionamento

```c
long pos = ftell(f);
fseek(f, 0L, SEEK_END);    // vai a fine file
long size = ftell(f);
fseek(f, 0L, SEEK_SET);    // torna all'inizio
```

`SEEK_SET`, `SEEK_CUR`, `SEEK_END` sono le tre "origini" del seek.

Per file grandi > 2 GB usa `ftello` / `fseeko` (POSIX).

## Errori e stato

- `feof(f)` → dopo una lettura, dice se hai raggiunto EOF. **Non** usarlo come condizione del loop "prima" di leggere: legge prima e controlla dopo.
- `ferror(f)` → errore di I/O.
- `clearerr(f)` → resetta gli stati di errore.
- `perror("msg")` → stampa "msg: spiegazione" usando `errno`.

## Stream standard come `FILE *`

`stdin`, `stdout`, `stderr` sono `FILE *`. Tutte le funzioni I/O hanno una variante `fXXX`:

```c
fprintf(stderr, "errore\n");
fgets(buf, sizeof buf, stdin);
```

## Buffering

- **Line-buffered** di default se `stdout` è un terminale.
- **Block-buffered** se rediretto a file.
- `stderr` è **unbuffered**.

Controlla con:
```c
setvbuf(f, NULL, _IONBF, 0);   // unbuffered
setvbuf(f, NULL, _IOLBF, 0);   // line-buffered
setvbuf(f, NULL, _IOFBF, 8192);// full-buffered con buffer da 8K
```

## Il pattern "cleanup":

```c
FILE *f = fopen(path, "r");
if (!f) { perror(path); return -1; }

char *buf = malloc(N);
if (!buf) { fclose(f); return -1; }

/* ... lavoro ... */

free(buf);
fclose(f);
return 0;
```

Con più risorse, il `goto cleanup` della lezione 05 delle fondamenta brilla.

## Esercizi

1. Scrivi un programma che legge un file testuale e conta righe, parole e caratteri (come `wc`).
2. Scrivi una funzione `char *leggi_tutto(const char *path, size_t *out_len)` che legge un intero file in memoria.
3. Scrivi un programma che copia un file in un altro (byte-per-byte) usando `fread`/`fwrite` con buffer da 4096.
4. Salva un array di `double` in binario, rileggilo e verifica l'identità bit a bit.
5. Scrivi una funzione che stampa gli ultimi N byte di un file (tail -c).
