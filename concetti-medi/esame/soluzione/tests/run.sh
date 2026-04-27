#!/usr/bin/env bash
# Test funzionali obbligatori dell'esame Concetti Medi.
# Richiede: ./kvdb gia' compilato nella directory superiore.

set -e

cd "$(dirname "$0")/.."

if [[ ! -x ./kvdb ]]; then
    echo "errore: ./kvdb non trovato. Lancia prima 'make'." >&2
    exit 1
fi

TMPDIR=$(mktemp -d)
trap "rm -rf '$TMPDIR'" EXIT

pass=0
fail=0

check() {
    local nome="$1" atteso="$2" attuale="$3"
    if [[ "$attuale" == "$atteso" ]]; then
        echo "  PASS  $nome"
        pass=$((pass + 1))
    else
        echo "  FAIL  $nome"
        echo "        atteso  : $atteso"
        echo "        ottenuto: $attuale"
        fail=$((fail + 1))
    fi
}

echo "-- Test 1: set/get di tutti i tipi --"
out=$(./kvdb 2>/dev/null <<EOF
set i_val i 42
set d_val d 3.14
set s_val s ciao mondo
get i_val
get d_val
get s_val
quit
EOF
)
check "set+get int"    "42"              "$(echo "$out" | sed -n 4p)"
check "set+get double" "3.14"            "$(echo "$out" | sed -n 5p)"
check "set+get string" '"ciao mondo"'    "$(echo "$out" | sed -n 6p)"

echo "-- Test 2: sovrascrittura chiave --"
out=$(./kvdb 2>/dev/null <<EOF
set k s primo
set k s secondo
get k
quit
EOF
)
check "overwrite"      '"secondo"'       "$(echo "$out" | sed -n 3p)"

echo "-- Test 3: round-trip save/load --"
./kvdb 2>/dev/null <<EOF > /dev/null
set pi d 3.141592653589793
set age i 30
set nome s Riccardo con spazi
save $TMPDIR/db.bin
quit
EOF
out=$(./kvdb 2>/dev/null <<EOF
load $TMPDIR/db.bin
get pi
get age
get nome
quit
EOF
)
check "roundtrip double" "3.14159"                   "$(echo "$out" | sed -n 2p)"
check "roundtrip int"    "30"                        "$(echo "$out" | sed -n 3p)"
check "roundtrip string" '"Riccardo con spazi"'      "$(echo "$out" | sed -n 4p)"

echo "-- Test 4: load di un file con magic errato --"
echo -n "FAKE" > "$TMPDIR/bogus.bin"
dd if=/dev/zero bs=1 count=12 >> "$TMPDIR/bogus.bin" 2>/dev/null
out=$(./kvdb <<EOF 2>&1
load $TMPDIR/bogus.bin
quit
EOF
)
if echo "$out" | grep -q "magic"; then
    echo "  PASS  magic invalido segnalato"
    pass=$((pass + 1))
else
    echo "  FAIL  magic invalido NON segnalato"
    echo "        output: $out"
    fail=$((fail + 1))
fi

echo "-- Test 5: list su store vuoto non crasha --"
out=$(./kvdb 2>/dev/null <<EOF
list
quit
EOF
)
check "list vuoto" "" "$out"

echo "-- Test 6: get/del su chiave inesistente --"
out=$(./kvdb 2>/dev/null <<EOF
get missing
del missing
quit
EOF
)
check "get missing" "(nil)"      "$(echo "$out" | sed -n 1p)"
check "del missing" "not found"  "$(echo "$out" | sed -n 2p)"

echo "-- Test 7: tipo invalido --"
out=$(./kvdb <<EOF 2>&1
set k z 1
get k
quit
EOF
)
if echo "$out" | grep -q "tipo 'z' non valido"; then
    echo "  PASS  tipo invalido rifiutato"
    pass=$((pass + 1))
else
    echo "  FAIL  tipo invalido non rifiutato"
    fail=$((fail + 1))
fi

echo
echo "Totale: $pass pass, $fail fail"
if [[ $fail -gt 0 ]]; then
    exit 1
fi
