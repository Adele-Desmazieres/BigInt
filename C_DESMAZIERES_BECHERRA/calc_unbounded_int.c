#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>


//A TESTER

typedef struct{
    char* token;
    node* next;
} node;

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