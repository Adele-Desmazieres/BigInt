CC=gcc
CFLAGS=		-Wall -g -pedantic
LDLIBS= 	-lm

ALL= calc_unbounded_int test_unbounded
all: $(ALL)


calc_unbounded_int: calc_unbounded_int.o unbounded_int.o
test_unbounded: test_unbounded.o unbounded_int.o

unbounded_int.o: unbounded_int.c unbounded_int.h
calc_unbounded_int.o: calc_unbounded_int.c unbounded_int.h
test_unbounded.o: test_unbounded.c unbounded_int.h


clean:
	rm -rf *~
cleanall:
	rm -rf *~ *.o $(ALL)