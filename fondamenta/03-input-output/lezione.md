# Lezione 03 — Input e Output

Nel C standard l'I/O è gestito dalla libreria `<stdio.h>` (standard I/O).

## I tre stream standard

Quando un programma parte, ha tre "canali" già aperti:

| Stream   | Default       | Uso                          |
|----------|---------------|------------------------------|
| `stdin`  | tastiera      | lettura input                |
| `stdout` | schermo       | output normale               |
| `stderr` | schermo       | errori e diagnostica         |

Da shell puoi redirigerli:
```bash
./programma < input.txt       # stdin da file
./programma > output.txt      # stdout su file
./programma 2> errori.txt     # stderr su file
```

## `printf` — stampa formattata

```c
printf("formato", arg1, arg2, ...);
```

Il **formato** contiene testo normale più `%` come segnaposto.

Specificatori principali:

| Specificatore | Tipo           | Esempio                            |
|---------------|----------------|------------------------------------|
| `%d` `%i`     | `int`          | `printf("%d", 42)`                 |
| `%u`          | `unsigned`     | `printf("%u", 42u)`                |
| `%ld`         | `long`         | `printf("%ld", 42L)`               |
| `%lld`        | `long long`    | `printf("%lld", 42LL)`             |
| `%f`          | `double`       | `printf("%f", 3.14)`               |
| `%.2f`        | `double`, 2 decimali | `printf("%.2f", 3.14)`       |
| `%e`          | notazione scientifica | `printf("%e", 1e6)`         |
| `%c`          | `char`         | `printf("%c", 'A')`                |
| `%s`          | stringa (`char*`) | `printf("%s", "ciao")`          |
| `%p`          | puntatore      | `printf("%p", (void*)ptr)`         |
| `%x` `%X`     | hex            | `printf("%x", 255)` → `ff`         |
| `%zu`         | `size_t`       | `printf("%zu", sizeof(int))`       |
| `%%`          | un `%` letterale | `printf("100%%")`                |

**ATTENZIONE**: se il tipo del valore non coincide con lo specificatore, è **UB**. Il compilatore con `-Wall` ti avverte: leggi sempre i warning.

Modificatori:
- Larghezza: `%5d` → almeno 5 caratteri.
- Allineamento: `%-5d` → a sinistra.
- Zero-padding: `%05d` → `00042`.
- Precisione: `%.3f`.

## `puts` e `putchar` — più semplici

```c
puts("ciao");        // stampa "ciao" e va a capo automaticamente
putchar('A');        // stampa un singolo carattere
```

Quando non hai bisogno di formattazione, `puts` è più sicuro e più veloce.

## `scanf` — input formattato (con prudenza)

```c
int x;
scanf("%d", &x);   // nota l'operatore & per passare l'indirizzo
```

`scanf` **è pieno di insidie**. In codice reale si preferiscono `fgets` + parsing manuale. Ma impariamo prima le basi.

### Regole per sopravvivere a `scanf`:
1. **Controlla sempre il valore di ritorno**: restituisce il numero di campi letti con successo.
2. **Mai** `%s` senza limite di lunghezza: usa `%99s` se il buffer è 100.
3. `%d`, `%f` eccetera saltano automaticamente whitespace iniziale; `%c` no.

```c
int n;
if (scanf("%d", &n) != 1) {
    fprintf(stderr, "input non valido\n");
    return 1;
}
```

## `fgets` — lettura di una riga (raccomandato)

```c
char buffer[128];
if (fgets(buffer, sizeof buffer, stdin) != NULL) {
    // buffer contiene la riga, incluso '\n' finale (se c'era spazio)
}
```

`fgets` legge fino a `size-1` caratteri, o fino a `\n`, o EOF. È il modo **sicuro** di leggere input testuale.

## `fprintf` — stampa su uno stream specifico

```c
fprintf(stderr, "Errore: file non trovato\n");
```

Gli errori vanno su `stderr`, non su `stdout`. Serve a distinguerli nelle pipeline e a non "sporcare" l'output.

## Buffering

`stdout` è tipicamente **bufferato per riga** se collegato a un terminale, **completamente** se rediretto a file. Se vuoi forzare la scrittura immediata:

```c
fflush(stdout);
```

Oppure metti `\n` nel messaggio (buffering a riga lo sfrutta).

## Esercizi

1. Chiedi nome ed età all'utente e stampa un saluto personalizzato.
2. Leggi due numeri e stampa la loro somma, differenza, prodotto e quoziente (attenzione ai tipi).
3. Usa `fgets` per leggere una riga e stamparla al contrario.
4. Stampa un errore su `stderr` e un messaggio su `stdout` nello stesso programma; redirigi solo uno dei due a file.
