CC = gcc

CFLAGS = -Wall -c -I /Library/Frameworks/SDL2.framework/Headers -I /Library/Frameworks/SDL2_image.framework/Headers
LDFLAGS = -framework SDL2 -framework SDL2_image
EXE = bin/game
SRC = src
OFILES = $(patsubst %.c,%.o,$(wildcard $(SRC)/*.c)) 

all: $(EXE)

$(EXE): $(OFILES)
	$(CC) $(LDFLAGS) $^ -o $@

$(SRC)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm $(SRC)/*.o && rm $(EXE)
