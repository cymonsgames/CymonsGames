BIN		=	nibbles
OBJ		=	main.o misc.o screen.o
LDFLAGS	=	-lncurses
CC		=	gcc -O2 -Wall

all: $(BIN)

clean:
	rm -f $(BIN) $(OBJ)

$(BIN): $(OBJ)
	$(CC) -o $(BIN) $(OBJ) $(LDFLAGS)
	
misc.o: misc.c misc.h config.h
	$(CC) -c -o $@ $<

main.o: main.c config.h misc.h screen.h
	$(CC) -c -o $@ $<

screen.o: screen.c config.h misc.h screen.h
	$(CC) -c -o $@ $<
