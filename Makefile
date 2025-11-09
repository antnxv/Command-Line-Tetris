CFLAGS := -I. -lncurses -lpthread

all: tetris display board player
	gcc tetris.o display.o board.o player.o -o tetris.out $(CFLAGS)

tetris: tetris.c tetris.h board.h player.h display.h
	gcc -c tetris.c $(CFLAGS)

display: display.c tetris.h board.h player.h display.h pieces.h
	gcc -c display.c $(CFLAGS)

board: board.c tetris.h board.h player.h display.h pieces.h
	gcc -c board.c $(CFLAGS)

player: player.c tetris.h board.h player.h display.h pieces.h
	gcc -c player.c $(CFLAGS)

run: all
	./tetris.out

clean:
	rm -f *.o
	rm -f tetris.out
