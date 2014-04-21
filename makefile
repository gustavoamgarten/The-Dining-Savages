CC = gcc
CFLAGS = -g -pthread -lcurses

PROGRAMS = thediningsavages

all: $(PROGRAMS)

clean:
	rm -f *~ *.o $(PROGRAMS)
