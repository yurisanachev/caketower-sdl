CC = gcc

CFLAGS = -Wall -c -I /Library/Frameworks/SDL2.framework/Headers
LDFLAGS = -framework SDL2
EXE = bin/game
SRC = src
CFILES = $(wildcard $(SRC)/*.c)
OFILES = $(wildcard $(SRC)/*.o)

all: $(EXE)

$(EXE): $(SRC)/game.o
	$(CC) $(LDFLAGS) $< -o $@

$(SRC)/game.o: $(CFILES)
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm $(OFILES) && rm $(EXE)
