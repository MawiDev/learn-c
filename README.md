# Corso di C Moderno

Benvenuto Riccardo. Questo è un corso strutturato di C moderno (standard **C11/C17**, con cenni a **C23**).
Non è una raccolta di trucchi: è un percorso. Segui le cartelle in ordine.

## Come studiare

1. Leggi `lezione.md` → è la teoria con le spiegazioni "da professore".
2. Apri `esempio.c` → compilalo ed eseguilo, leggilo riga per riga.
3. Apri `esercizi.c` → risolvi gli esercizi da solo, poi confronta.

## Compilatore

Userai `gcc` (o `clang`). Standard consigliato **C17** con tutti i warning attivi:

```bash
gcc -std=c17 -Wall -Wextra -Wpedantic -O2 esempio.c -o esempio
./esempio
```

Questi flag non sono opzionali: i warning del compilatore ti insegnano più di qualsiasi libro.

## Programma del corso

### Fondamenta — [fondamenta/](fondamenta/)
- [01 — Introduzione e compilazione](fondamenta/01-introduzione/lezione.md)
- [02 — Variabili e tipi](fondamenta/02-variabili-e-tipi/lezione.md)
- [03 — Input / Output](fondamenta/03-input-output/lezione.md)
- [04 — Operatori](fondamenta/04-operatori/lezione.md)
- [05 — Controllo di flusso](fondamenta/05-controllo-flusso/lezione.md)
- [06 — Cicli](fondamenta/06-cicli/lezione.md)
- [07 — Funzioni](fondamenta/07-funzioni/lezione.md)
- [08 — Array](fondamenta/08-array/lezione.md)
- [09 — Stringhe](fondamenta/09-stringhe/lezione.md)
- [10 — Puntatori (basi)](fondamenta/10-puntatori-base/lezione.md)
- **[Esame: Calcolatrice RPN](fondamenta/esame/esame.md)** — progetto finale

### Concetti Medi — [concetti-medi/](concetti-medi/)
- [01 — Puntatori avanzati](concetti-medi/01-puntatori-avanzati/lezione.md)
- [02 — Memoria dinamica](concetti-medi/02-memoria-dinamica/lezione.md)
- [03 — Strutture](concetti-medi/03-strutture/lezione.md)
- [04 — Union ed Enum](concetti-medi/04-union-enum/lezione.md)
- [05 — File I/O](concetti-medi/05-file-io/lezione.md)
- [06 — Preprocessore e macro](concetti-medi/06-preprocessore-e-macro/lezione.md)
- [07 — Puntatori a funzione](concetti-medi/07-puntatori-a-funzione/lezione.md)
- [08 — Compilazione multi-file](concetti-medi/08-compilazione-multifile/lezione.md)
- [09 — Tipi moderni (C11/C17/C23)](concetti-medi/09-tipi-moderni/lezione.md)
- [10 — Gestione degli errori](concetti-medi/10-gestione-errori/lezione.md)
- **[Esame: Key-Value Store persistente](concetti-medi/esame/esame.md)** — progetto finale multifile

## Esami

Ogni livello ha un **progetto finale** impegnativo che mette in pratica **tutte** le lezioni del livello:

### [Esame Fondamenta — Calcolatrice RPN](fondamenta/esame/)
Implementa un interprete RPN (Reverse Polish Notation) con stack, tokenizer a puntatori, operatori aritmetici (`+ - * / %`), comandi (`dup drop swap clear print help quit`), modulo floating-point scritto a mano, gestione errori robusta.

- [`esame.md`](fondamenta/esame/esame.md) — consegna con specifiche, criteri, test
- [`starter/rpn.c`](fondamenta/esame/starter/rpn.c) — scheletro con TODO da riempire
- [`soluzione/rpn.c`](fondamenta/esame/soluzione/rpn.c) — soluzione di riferimento

### [Esame Concetti Medi — KV Store persistente](concetti-medi/esame/)
Implementa un database key-value stile mini-Redis: tagged union per i valori (`int64 | double | string`), storage dinamico, serializzazione binaria little-endian con magic + versione, dispatch dei comandi via puntatori a funzione, logging strutturato con livelli, progetto multifile compilato via `make`.

- [`esame.md`](concetti-medi/esame/esame.md) — consegna con specifica del formato binario, vincoli, test obbligatori
- [`starter/`](concetti-medi/esame/starter/) — scheletro multifile con header completi e stub
- [`soluzione/`](concetti-medi/esame/soluzione/) — soluzione completa + test suite in [`tests/run.sh`](concetti-medi/esame/soluzione/tests/run.sh)

Compila con `make` (produzione) o `make asan` (con AddressSanitizer attivo per debug). Lancia i test con `./tests/run.sh`.

## Filosofia del corso

> Il C non ti protegge. Ti dà il controllo.
> Ogni regola che imparerai esiste per un motivo: qualcuno ci ha rimesso memoria, sicurezza o sanità mentale.

Scriveremo codice che:
- compila pulito con `-Wall -Wextra -Wpedantic`,
- non perde memoria,
- non accede fuori dai limiti,
- si legge come prosa.
