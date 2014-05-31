CC=gcc
CFLAGS=-Wall -g
LFLAGS=-g -lm

OFILES=main.o game.o

TAWS_v5: $(OFILES)
	$(CC) -o $@ $(OFILES) $(LFLAGS)

.c.o:
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -f TAWS_v5 $(OFILES)
