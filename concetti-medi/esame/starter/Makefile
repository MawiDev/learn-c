CC      = gcc
CFLAGS  = -std=c17 -Wall -Wextra -Wpedantic -Werror -O2 -g
LDFLAGS =

SRC  = main.c log.c store.c format.c commands.c
OBJ  = $(SRC:.c=.o)
DEPS = common.h log.h store.h format.h commands.h
BIN  = kvdb

.PHONY: all clean asan

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

# Build con sanitizer per debug
asan: CFLAGS = -std=c17 -Wall -Wextra -Wpedantic -g -O1 -fsanitize=address,undefined
asan: LDFLAGS = -fsanitize=address,undefined
asan: $(BIN)

clean:
	rm -f $(OBJ) $(BIN)
