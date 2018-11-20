IDIR=headers
CC=gcc
LDIR =lib

CFLAGS=-I$(IDIR)
LIBS=-ljabra

battstat: src/main.c 
	$(CC) -o battstat src/main.c $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f battstat
