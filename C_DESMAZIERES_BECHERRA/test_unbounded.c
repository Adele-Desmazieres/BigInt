
#include "unbounded_int.h"



//MAIN

int main(void){

	//Tests
	unbounded_int test1=string2unbounded_int("0");
	print_unbound(test1,0);
	print_unbound(test1,0);
	print_unbound(test1,0);
	
	unbounded_int test2=ll2unbounded_int(-45235);
	print_unbound(test2,0);
	
	char* test3=unbounded_int2string(test2);
	char* test3bis=unbounded_int2string(test1);
	printf("%s\n",test3);
	printf("%s\n",test3bis);

	printf("---------------------------------\n\n");
	printf("Comparaison 1: %d\n", unbounded_int_cmp_unbounded_int(test2,test1));

	printf("Comparaison 2: %d\n", unbounded_int_cmp_ll(test2,-8));
	printf("---------------------------------\n\n");

	unbounded_int add=unbounded_int_somme(test1,test2);
	print_unbound(add, 0);
	printf("---------------------------------\n\n");

	unbounded_int subs=unbounded_int_difference(test1,test2);
	print_unbound(subs,0);
	printf("---------------------------------\n\n");

	unbounded_int mult=unbounded_int_produit(test1,test2);
	print_unbound(mult,0);
	
	return 0;
}