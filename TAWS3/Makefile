CC=gcc
CFLAGS=-Wall -g
LFLAGS=-g

OFILES=main.o game.o

TAWS_v3: $(OFILES)
	$(CC) -o $@ $(OFILES) $(LFLAGS)

.c.o:
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -f TAWS_v3 $(OFILES)
