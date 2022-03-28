#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

typedef struct chiffre chiffre;
//typedef struct unbounded_int unbounded_int;

//La structure d'un noeud
typedef struct chiffre{
    chiffre *suivant;
    char c;
    chiffre *precedent;
} chiffre;

//La structure
typedef struct{
    char signe; /* soit ’+’ soit ’-’ */
    size_t len; /* longueur de la liste */
    chiffre *premier; /* pointeur vers le premier élément de la liste*/
    chiffre *dernier; /*pointeur vers le dernier élément de la liste*/
} unbounded_int;


//FONCTIONS UTILITAIRES

//Pour imprimer un unbounded int joliement
void printUnbound(unbounded_int l, int debug);


//FONCTIONS DU SUJET

unbounded_int string2unbounded_int( const char *e );

unbounded_int ll2unbounded_int (long long i);

char* unbounded_int2string(unbounded_int i);

int unbounded_int_cmp_unbounded_int(unbounded_int a, unbounded_int b);

int unbounded_int_cmp_ll(unbounded_int a, long long b);

//Déclaration de la différence
unbounded_int unbounded_int_difference( unbounded_int a, unbounded_int b);

unbounded_int unbounded_int_somme(unbounded_int a, unbounded_int b);

unbounded_int unbounded_int_difference( unbounded_int a, unbounded_int b);

unbounded_int unbounded_int_produit( unbounded_int a, unbounded_int b);