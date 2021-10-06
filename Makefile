CC=gcc
CFLAGS=-Wall -ggdb
OBJECTS=license.o common.o

default: runsim testsim

runsim: $(OBJECTS) runsim.c
	$(CC) $(CFLAGS) -o runsim runsim.c $(OBJECTS)

testsim: testsim.c common.o  license.o
	$(CC) $(CFLAGS) -o testsim testsim.c $(OBJECTS)

license.o: license.c license.h
	$(CC) $(CFLAGS) -c license.c

common.o: common.c common.h
	$(CC) $(CFLAGS) -c common.c

clean:
	rm $(OBJECTS) runsim testsim
