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

//Renvoie la longueur d'une chaine de caractère
static size_t len(const char* c){
    int len=0;
    for(;*c!='\0';c++,len++){}
    return len;
}

//Teste si la chaîne c est un nombre
static int isNumber(const char* c){
    if(*c!='-' && !isdigit(*c)) return 0;
    if(*c=='-'){
        c++;
        if(*c=='\0') return 0;
    }
    
    for(; *c!='\0';c++){
        if(!isdigit(*c)) return 0;
    }
    return 1;
}

//Constructeur de chiffre
static chiffre* newChiffre(char d){
    chiffre* ret=malloc(sizeof(chiffre));
    if(ret==NULL) exit(1);
    ret->c=d;
    ret->precedent=NULL;
    ret->suivant=NULL;
    return ret;
}

//Constructeur de unbounded_int
static unbounded_int* newUnbound(){
    unbounded_int* ret=malloc(sizeof(unbounded_int));
    if(ret==NULL) exit(1);
    ret->len=0;
    ret->signe='+';
    ret->premier=NULL;
    ret->dernier=NULL;
    return ret;
}

//Insertion d'un numéro c dans la liste l
static void push(char c, unbounded_int* l){

    if(l->premier==NULL){
        //Mise à jour de unbounded int
        l->premier=newChiffre(c);
        l->dernier=l->premier;
    } else {
        chiffre* tmp=l->premier;
        while(tmp->suivant!=NULL){
            tmp=tmp->suivant;
        }
        //Mise à jour de la liste ici
        tmp->suivant=newChiffre(c);
        l->dernier->suivant=tmp->suivant;
        l->dernier=tmp->suivant;
        l->dernier->precedent=tmp;
    }

    l->len++;
    
}

//Pour imprimer un chiffre
static void printChiffre(chiffre f, int debug){
    printf("%c", f.c);
    if(debug){
        printf(" Suivant: %p Precedent: %p\n",f.suivant,f.precedent);
    }
}

//Pour imprimer un unbounded int joliement
static void printUnbound(unbounded_int l, int debug){
    printf("Signe: %c   Longueur: %lld\n", l.signe,l.len);
    chiffre* tmp=l.premier;
    for(;tmp!=NULL; tmp=tmp->suivant){
        printChiffre(*tmp,debug);
    }
    printf("\n");
}

//Pour transformer un long long i en chaîne de caractères
static char* ll2string(long long i){

    double log=log10(abs(i));
    int Ilen=(int)log;

    char* ret=malloc((Ilen+1)*sizeof(char));
    if(ret==NULL) exit(1);

    //Méthode qui convertit un long long i en chaine de caractère stockée à ret, en base 10
    lltoa(i,ret,10);

    return ret;
}






//FONCTIONS A IMPLEMENTER


unbounded_int* string2unbouded_int( const char *e ){

    //On vérifie que la chaîne soit bien une représentation d'un nombre
    if(len(e)==0 || isNumber(e)==0) return NULL;

    //On initialise un unbounded_int
    unbounded_int* ret=newUnbound();

    //On regarde le signe du nombre
    if(*e=='-'){
        ret->signe='-'; e++;
    } 

    //On insère tous les caractères du nombre dans la liste
    for(; *e!='\0';e++){
        push(*e,ret);
    }

    //On renvoie le résultat
    return ret;
}

unbounded_int* ll2unbounded_int (long long i){

    //On convertit le long long en chaîne de caractères
    char* num=ll2string(i);

    //Puis on renvoie la fonction précédente avec cette chaîne
    return string2unbouded_int(num);
}

char* unbounded_int2string(unbounded_int i){

    //On alloue la mémoire à la chaîne qu'on va renvoyer
    char* ret=malloc((i.len+1)*sizeof(char));
    if(ret==NULL) exit(1);

    //On garde en mémoire le pointeur du début de la chaîne
    char* ret2=ret;

    //On itère sur la liste dans i pour renvoyer ajouter à la chaîne
    if(i.premier==NULL) { ret[0]='\0'; return ret; }
    else {
        if(i.signe=='-') { ret[0]='-'; ret++; }
        chiffre* tmp=i.premier;
        for(; tmp!=NULL ; ret++, tmp=tmp->suivant ){
            *ret=tmp->c;
        }
        *ret='\0';
    }

    //On renvoie le pointeur du début de la chaîne
    return ret2;
}

int unbounded_int_cmp_unbounded_int(unbounded_int a, unbounded_int b){

    //On teste pour voir si un des unbounded int est vide
    if(a.premier==NULL || b.premier==NULL) return -2;

    //On compare maintenant les signes des entiers (et init. des variables de retour pour signes)
    int ret=1; int ret2=-1;
    if(a.signe!=b.signe){
        //Cas particulier du 0
        if(a.len==1 && b.len==1 && a.premier->c==0 && b.premier->c==0) return 0;
        if(a.signe=='-' && b.signe=='+') return -1;
        else return 1;
    }

    //Si les entiers sont négatifs, alors on inverse les valeurs de retour
    if(a.signe=='-' && b.signe=='-'){
        ret=-1; ret2=1;
    }
    
    //Si ce n'est pas le cas, alors on itère et on compare
    chiffre *ta= a.premier;
    chiffre *tb= b.premier;
    for(;ta!=NULL && tb!=NULL; ta=ta->suivant, tb=tb->suivant){
        if(ta->c>tb->c) return ret;
        if(ta->c<tb->c) return ret2;
    }
    return 0;
}

int unbounded_int_cmp_ll(unbounded_int a, long long b){

    //On transforme le long long en unbounded int, puis on utilise la fonction précédente
    unbounded_int* c=ll2unbounded_int(b);
    return unbounded_int_cmp_unbounded_int(a,*c);
}

//MAIN

int main(void){

    //Tests
    unbounded_int* test1=string2unbouded_int("-4");
    printUnbound(*test1,0);
    printUnbound(*test1,0);
    printUnbound(*test1,0);
    
    unbounded_int* test2=ll2unbounded_int(-8);
    printUnbound(*test2,0);
    
    char* test3=unbounded_int2string(*test2);
    char* test3bis=unbounded_int2string(*test1);
    printf("%s\n",test3);
    printf("%s\n",test3bis);

    printf("Comparaison 1: %d\n", unbounded_int_cmp_unbounded_int(*test2,*test1));

    printf("Comparaison 2: %d\n", unbounded_int_cmp_ll(*test2,-8));
}