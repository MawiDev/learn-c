# Esame Concetti Medi — Key-Value Store persistente (mini-Redis)

**Durata consigliata**: 1–2 giornate. Progetto a più file.

## Obiettivo

Implementare un **key-value store in-memory** con:

- Chiavi **stringa**.
- Valori **tipizzati** tramite *tagged union* (intero a 64 bit, double, stringa).
- Interfaccia testuale a riga di comando tramite **dispatch table** di puntatori a funzione.
- **Persistenza** su file binario con formato ben definito.
- **Logging** strutturato con livelli e macro che iniettano `__FILE__`/`__LINE__`/`__func__`.
- **Progetto multifile** con header ben separati, compilato con `make`.

Questo esame copre **tutte e 10 le lezioni** dei concetti medi. Nessuna è opzionale.

## Specifica del protocollo interattivo

Il programma legge comandi da `stdin` (una riga alla volta). Ogni riga è:

```
<comando> [argomenti...]
```

| Comando                          | Descrizione                                                   |
|----------------------------------|---------------------------------------------------------------|
| `set <chiave> <tipo> <valore>`  | Inserisce/aggiorna. `<tipo>` ∈ `{i,d,s}`                       |
| `get <chiave>`                   | Stampa il valore (o `(nil)` se non esiste)                    |
| `del <chiave>`                   | Elimina la voce, stampa `OK` o `not found`                    |
| `list`                           | Stampa tutte le coppie, una per riga                          |
| `save <file>`                    | Serializza lo store corrente su file binario                  |
| `load <file>`                    | Sostituisce lo store con quello letto dal file                |
| `help`                           | Stampa gli help di ogni comando                               |
| `quit`                           | Termina il programma (salvando se hai implementato auto-save) |

### Formato dei valori

- `i` → intero a 64 bit (`int64_t`), rappresentato in decimale: `set age i 30`
- `d` → `double`: `set pi d 3.14159`
- `s` → stringa: `set nome s Riccardo` — il valore è **tutto ciò che segue** sulla riga (spazi inclusi), trimmato a sinistra.

### Esempio di sessione

```
> set nome s Riccardo Savio
OK
> set age i 30
OK
> set altezza d 1.82
OK
> list
nome    = string  "Riccardo Savio"
age     = int     30
altezza = double  1.82
> save /tmp/db.bin
OK (3 voci scritte, 57 byte)
> del age
OK
> load /tmp/db.bin
OK (3 voci caricate)
> get age
30
> quit
```

## Specifica del formato binario

Il file è little-endian. Deve iniziare con magic `"KVDB"` e una versione.

```
Offset  Dim   Campo
------- ----  ---------------------------------
0       4     magic "KVDB" (ASCII, no null)
4       4     version (uint32_t LE) = 1
8       4     count   (uint32_t LE)
12      ...   record[0], record[1], ... record[count-1]

record:
  0       4   key_len  (uint32_t LE)
  4       K   key_bytes (K = key_len, no null)
  4+K     1   kind     (uint8_t): 0=INT, 1=DOUBLE, 2=STRING
  5+K     ...   dato secondo il tipo:
                   INT:    8 byte, int64_t LE
                   DOUBLE: 8 byte, IEEE-754 binary64 LE
                   STRING: 4 byte len (uint32_t LE) + len byte
```

Aggiungi `_Static_assert` per verificare a compile-time che `sizeof(double) == 8` e `sizeof(int64_t) == 8`.

Assumiamo host **little-endian IEEE-754** (verificarlo a runtime con un `assert` è un bonus).

## Vincoli architetturali (obbligatori)

Il progetto **deve** essere suddiviso in questi file:

```
progetto/
├── Makefile
├── main.c          — REPL, parsing riga, dispatch
├── common.h        — tipi comuni, macro ARRAY_SIZE, _Static_assert
├── log.h / log.c   — mini-logger con livelli e variadic macro
├── store.h / store.c — KVStore, Value, operazioni CRUD
├── format.h / format.c — save/load binario
└── commands.h / commands.c — tabella comandi con puntatori a funzione
```

## Requisiti di codice

1. **Zero memory leak** (verificabile con `valgrind --leak-check=full` o ASan).
2. **Zero UB**. Compila con `-fsanitize=address,undefined` e fai girare i test: nessuna diagnostica.
3. **Compilazione pulita** con `-std=c17 -Wall -Wextra -Wpedantic -Werror`.
4. **Cleanup single-exit** via `goto` nelle funzioni `save` e `load` (e ovunque abbia senso).
5. **Logging**: `LOG_DEBUG`, `LOG_INFO`, `LOG_WARN`, `LOG_ERROR`. Le macro devono:
   - iniettare `__FILE__`, `__LINE__`, `__func__`,
   - essere *variadiche*,
   - rispettare la soglia corrente (`log_set_level(level)`).
6. **Dispatch table** dei comandi: **obbligatorio** implementarla come array di struct `{ nome, handler, help }` con `handler` puntatore a funzione. Niente `if/else if` a cascata.
7. **Tagged union** per `Value`. Usa `anonymous union` (C11) per leggibilità.
8. **Nessun uso di `strtok`** (non è rientrante). Usa `strtok_r` (POSIX) o scrivi il tuo tokenizer.
9. **Ownership**: `store` possiede (owns) tutte le stringhe (chiavi e valori `STRING`). `store_free` libera tutto. Ogni `set` che sovrascrive una chiave esistente deve liberare il valore precedente.
10. **Gestione errori coerente**: le funzioni tornano `int` (`0` = ok, `< 0` = errore), tranne helper che ritornano puntatori (`NULL` = errore).
11. **`_Generic`** (lezione 9) **bonus**: implementa una macro `value_make(x)` che costruisce un `Value` dal tipo C (`int64_t` → INT, `double` → DOUBLE, `char *` → STRING). Usalo internamente al posto di boilerplate.

## Test obbligatori

Crea un piccolo script `tests/run.sh` che esegue almeno:

1. Set/get/del di tutti e tre i tipi.
2. Sovrascrittura di una chiave (verifica assenza di leak).
3. Save → restart → load → verifica round-trip bit-perfect di `double` (salva, carica, confronta con `memcmp`).
4. Caricamento di un file con magic number errato → deve fallire pulitamente con errore, NON crashare.
5. `list` su store vuoto → non stampa nulla ma non crasha.
6. Chiave inesistente → `get` stampa `(nil)`, `del` stampa `not found`.
7. `set` con tipo invalido (`z`) → errore, store invariato.

## Criteri di valutazione

| Area                                       | Peso |
|--------------------------------------------|------|
| Correttezza funzionale (tutti i comandi)   | 20%  |
| Formato binario + portabilità endianess    | 15%  |
| Gestione memoria (zero leak, zero UAF)     | 20%  |
| Architettura multifile e ownership chiari  | 15%  |
| Dispatch via puntatori a funzione          | 10%  |
| Logging e gestione errori coerenti         | 10%  |
| Uso idiomatico del C moderno (stdint, _Generic, _Static_assert, goto-cleanup) | 10% |

## Cosa consegnare

La cartella [starter/](starter/) contiene lo **scheletro** multifile con header completi, funzioni stub e Makefile. Riempilo.

La cartella [soluzione/](soluzione/) contiene la **soluzione di riferimento** completa e compilabile. Consulta **solo dopo** aver tentato. Non copiare: rileggi, capisci, riscrivi nel tuo stile.

## Bonus (facoltativi ma formativi)

- **Auto-save** su `quit` in un file configurabile via `argv[1]`.
- **Controllo di integrità**: aggiungi un CRC32 in coda al file.
- **Compaction**: dopo N `del`, ricompatta lo storage.
- **`[[nodiscard]]`** (C23) su tutte le funzioni che ritornano errore.
- **Benchmark** con `clock_gettime(CLOCK_MONOTONIC, ...)` di 1M `set`/`get`.
