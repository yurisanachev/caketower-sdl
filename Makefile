CC = gcc
# If your compiler is a bit older you may need to change -std=c++11 to -std=c++0x
CFLAGS = -Wall -c -I /Library/Frameworks/SDL2.framework/Headers
LDFLAGS = -framework SDL2
EXE = game

all: $(EXE)

$(EXE): main.o
	$(CC) $(LDFLAGS) $< -o $@

main.o: main.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm *.o && rm $(EXE)
