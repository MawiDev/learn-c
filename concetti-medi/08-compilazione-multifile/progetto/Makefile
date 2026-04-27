CC      = gcc
CFLAGS  = -std=c17 -Wall -Wextra -Wpedantic -O2 -g
OBJ     = main.o matematica.o
TARGET  = prog

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

%.o: %.c matematica.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
