#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include "unbounded_int.h"


//A TESTER

//Code pour les listes de variables

//Structure noeud de varList
typedef struct{
    unbounded_int value;
    char* name;
    node* next;
} nodeVar;

//Structure varList
typedef struct{
    size_t len;
    nodeVar* first;
} varList;

//Surcharge constructeur nodeVar avec unbounded int
nodeVar* newNodeVar(const char* c, unbounded_int val){

    //On init. le noeud
    nodeVar* ret=malloc(sizeof(nodeVar));
    if (ret==NULL){
        printf("Erreur constructeur nodeVar\n");
        exit(1);
    }

    ret->value=val;
    ret->name=c;
    ret->next=NULL;

    return ret;
}

//Constructeur de varList
varList* newVarList(){

    //Init. de retour
    varList* ret=malloc(sizeof(varList));
    if (ret==NULL){
        printf("Erreur constructeur varList\n");
        exit(1);
    }

    ret->len=0;
    ret->first=NULL;

    return ret;
}

//Teste l'égalité entre deux chaînes de caractère
int stringEquals(const char* a, const char* b){
    if(a==NULL || b==NULL){
        printf("Erreur comparaison chaîne nulle\n"); return 0;
    } 

    for(;*a!='\0' && b!='\0'; a++, b++){
        if(*a!=*b) return 0;
    }
    return 1;
}

//Trouve et renvoie un nodeVar de nom name si il existe dans la liste l, renvoie NULL sinon
nodeVar* findInList(const char* name, varList* l){
    if(l==NULL || name==NULL){
        printf("Erreur liste nulle ou nom nul\n"); return NULL;
    } 

    for(nodeVar* tmp=l->first; tmp!=NULL; tmp=tmp->next ){
        if(stringEquals(name,tmp->name)) return tmp;
    }
    return NULL;
}

//Push une variable sur l (doit commencer par une lettre) OU mettre a jour une variable existante si elle existe déjà
void varPush(const char* name, unbounded_int val, varList* l){
    //TODO
    if(l==NULL || name==NULL ||val.signe=='*'){
        printf("Erreur liste nulle, nom nulle ou nombre incorrect\n"); return;
    }

    //Si la variable n'existe pas, on la crée puis on l'ajoute à la liste
    if(findInList(name, l)==NULL){
        nodeVar* tmp=l->first;
        while(tmp->next!=NULL){
            tmp=tmp->next;
        }
        tmp->next=newNodeVar(name,val);
        l->len++;
    }

    //TODO

}




//Code pour les listes de string

//Structure noeud de stringList
typedef struct{
    char* token;
    node* next;
} node;

//Structure stringList
typedef struct{
    size_t len;
    node* first;
} stringList;

//Constructeur liste de Node
node* newNode(const char* c, int len){

    //Si la taille est nulle alors pb
    if(len==0) {
        printf("Erreur chaîne nulle\n");
        return NULL;
    }

    //On forme la chaîne de caractère depuis la ligne
    char* tok= malloc(len*sizeof(char));
    for(int i=0;i<len;i++){
        tok[i]=c[i];
    }

    //On init. le noeud
    node* ret=malloc(sizeof(node));
    if (ret==NULL){
        printf("Erreur constructeur node\n");
        exit(1);
    }

    ret->token=tok;
    ret->next=NULL;

    return ret;
}

//Constructeur de liste de string
stringList* newStringList(){

    stringList* ret=malloc(sizeof(stringList));
    if (ret==NULL){
        printf("Erreur constructeur stringList\n");
        exit(1);
    }

    ret->len=0;
    ret->first=NULL;

    return ret;
}

//Push une chaîne de caractère c sur l
void stringPush(const char* c, int len, stringList* l){
    node* noeud=newNode(c, len);

    if(l->first==NULL){
        l->first=noeud;
    } else {
        node* tmp=l->first;
        for(; *tmp->next!=NULL; tmp=tmp->next){
        }
        tmp->next=noeud;
    }

    l->len++;
}














//On transforme une ligne 'crue' (directement importée) en stringList d'arguments
stringList line2string_list(const char* rawLine){

    //ON intitialise le retour
    stringList* ret=newStringList();

    //On parcourt la ligne jusqu'à la fin (bon caractère?)
    while(*rawLine!='\n'){
        
        if(*rawLine!=' '){
            //On détermine la longueur de la chaîne
            int len=0;
            for(char* tmp=rawLine;*tmp!=' ';tmp++){
                len++;
            }

            //On ajoute la chaîne à la liste
            stringPush(rawLine,len,ret);

            //On skip la chaîne trouvée pour éviter de créer des immondices
            rawLine=rawLine+len;
        }

        rawLine++;
    }

    return *ret;
}