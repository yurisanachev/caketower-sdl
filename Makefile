CC = gcc

CFLAGS = -Wall -c -I /Library/Frameworks/SDL2.framework/Headers -I /Library/Frameworks/SDL2_image.framework/Headers
LDFLAGS = -framework SDL2 -framework SDL2_image
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
