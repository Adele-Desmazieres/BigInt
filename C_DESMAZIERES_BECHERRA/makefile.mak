prog: test_unbounded.o calc_unbounded.o unbounded_int.o
	gcc -o prog test_unbounded.o calc_unbounded.o unbounded_int.o

test_unbounded.o : test_unbounded.c
	gcc -Wall -g -pedantic test_unbounded.c

calc_unbounded.o : calc_unbounded.c
	gcc -Wall -g -pedantic calc_unbounded.c

unbounded_int.o : unbounded_int.c
	gcc -Wall -g -pedantic unbounded_int.c

clean :
	rm -f prog *.o 
