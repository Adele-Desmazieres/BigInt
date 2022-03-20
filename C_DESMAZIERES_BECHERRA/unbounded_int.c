#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

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
size_t len(const char* c){
    int len=0;
    for(;*c!='\0';c++,len++){}
    return len;
}

//Teste si la chaîne c est un nombre
int isNumber(const char* c){
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
chiffre* newChiffre(char d){
    chiffre* ret=malloc(sizeof(chiffre));
    if(ret==NULL) exit(1);
    ret->c=d;
    ret->precedent=NULL;
    ret->suivant=NULL;
    return ret;
}

//Constructeur de unbounded_int
unbounded_int* newUnbound(){
    unbounded_int* ret=malloc(sizeof(unbounded_int));
    if(ret==NULL) exit(1);
    ret->len=0;
    ret->signe='+';
    ret->premier=NULL;
    ret->dernier=NULL;
    return ret;
}

//Insertion d'un numéro c dans la liste l
void push(char c, unbounded_int* l){

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
void printChiffre(chiffre f, int debug){
    printf("%c", f.c);
    if(debug){
        printf(" Suivant: %p Precedent: %p\n",f.suivant,f.precedent);
    }
}

//Pour imprimer un unbounded int joliement
void printUnbound(unbounded_int l, int debug){
    printf("Signe: %c   Longueur: %lld\n", l.signe,l.len);
    chiffre* tmp=l.premier;
    for(;tmp!=NULL; tmp=tmp->suivant){
        printChiffre(*tmp,debug);
    }
    printf("\n");
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

int main(void){
    unbounded_int* test1=string2unbouded_int("12345678");
    printUnbound(*test1,0);
    printUnbound(*test1,0);
    printUnbound(*test1,0);
}