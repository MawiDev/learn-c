# Esame Fondamenta — Calcolatrice RPN

**Durata consigliata**: 4–6 ore. Niente libreria oltre a `<stdio.h>`, `<stdlib.h>`, `<string.h>`, `<ctype.h>`, `<stdbool.h>`, `<stddef.h>`.

## Obiettivo

Implementare una **calcolatrice in notazione polacca inversa** (Reverse Polish Notation — RPN) interattiva. L'utente digita una riga contenente **token separati da whitespace**; ogni token è:

- un **numero** in virgola mobile (es. `3.14`, `-2`, `1e-3`),
- un **operatore** binario: `+ - * / %`
- un **comando**:
  - `dup`   — duplica l'elemento in cima allo stack,
  - `drop`  — rimuove l'elemento in cima,
  - `swap`  — scambia i due in cima,
  - `clear` — svuota lo stack,
  - `print` — stampa lo stack (equivalente a ciò che fa a fine riga),
  - `help`  — stampa un messaggio di aiuto,
  - `quit`  — termina il programma.

Tutto ciò che segue `#` sulla riga è un **commento** e va ignorato.

Lo stack è composto da `double` e ha capacità massima `STACK_MAX = 64`.

## Specifiche funzionali

1. **Leggi una riga alla volta** con `fgets` da `stdin` fino a EOF o al comando `quit`.
2. Dopo aver processato **tutti i token di una riga**, stampa lo stack dal basso verso l'alto, separato da spazi. Esempio:
   ```
   > 3 4 +
   [ 7 ]
   > 2 *
   [ 14 ]
   > 10 /
   [ 1.4 ]
   ```
3. Se un token è un **numero**, lo esegui con `strtod` e controlli che **tutta** la stringa sia stata consumata (altrimenti è un token malformato).
4. Gli operatori binari fanno pop di 2, applicano l'operazione (`a op b` dove `b` è stato poppato per primo), e push del risultato.
5. `%` (modulo) opera su `double` via `fmod` — NON puoi includere `<math.h>` in questo esame: **implementalo a mano** su base `trunc` di `a/b`. Hint: `a - b * (long)(a/b)` va bene se gestisci segni correttamente.
6. **Errori** da segnalare su `stderr` e **abortire la riga corrente** (senza uscire dal programma):
   - token sconosciuto,
   - stack underflow (operazione con troppi pochi elementi),
   - stack overflow (push oltre `STACK_MAX`),
   - divisione per zero (`/` e `%`).
7. Dopo un errore, lo stack resta nello stato pre-errore: NON devi "rollbackare" pop già fatti all'interno dello stesso operatore — ma il tuo design deve evitare questo caso leggendo i valori **prima** di modificare lo stack (hint: usa `peek` o fai le pop solo dopo aver verificato le precondizioni).
8. La tokenizzazione deve essere **scritta a mano** con puntatori e `isspace` — **non** puoi usare `strtok`.

## Vincoli tecnici

- Compila pulito con `-std=c17 -Wall -Wextra -Wpedantic -Werror`.
- Nessun buffer overflow. `-fsanitize=address,undefined` non deve segnalare nulla.
- Usa `const` correttamente sui parametri di sola lettura.
- Usa `size_t` per dimensioni/indici, `bool` dove appropriato.
- Dichiara `static` le funzioni non esportate.
- Nessuna variabile globale modificabile (eccetto `argv[0]` per messaggi di errore se lo usi).
- `main` deve essere ≤ 50 righe. Delega tutto a funzioni.

## Criteri di valutazione

| Area                         | Peso |
|------------------------------|------|
| Correttezza numerica         | 25%  |
| Robustezza agli errori       | 20%  |
| Qualità della tokenizzazione | 15%  |
| Separazione in funzioni      | 15%  |
| Uso idiomatico del C moderno | 15%  |
| Compilazione pulita + sanitizer | 10% |

## Test minimi

Usa `stdin << HERE-DOC` per testare:

```bash
./rpn <<'EOF'
3 4 +
2 *
dup *
sqrt        # token sconosciuto -> errore, prosegue
clear
10 0 /      # divisione per zero -> errore
42 17 %
quit
EOF
```

Output atteso (indicativo):
```
[ 7 ]
[ 14 ]
[ 196 ]
errore: token sconosciuto 'sqrt'
[ 196 ]
[ ]
errore: divisione per zero
[ ]
[ 8 ]
```

## Consegna

Un singolo file `rpn.c` nella tua cartella `starter/` (dovrai trasformarlo in una soluzione funzionante).

Nella cartella [soluzione/](soluzione/) trovi una **soluzione di riferimento** da consultare **solo dopo aver provato**. Confronta il tuo approccio con quello esemplare: stile, error handling, struttura. Non copiare.
