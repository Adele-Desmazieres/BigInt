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

//Constructeur nodeVar avec unbounded int
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
nodeVar* findInList(const char* name, varList l){
    if(l.first==NULL || name==NULL){
        printf("Erreur liste nulle ou nom nul\n"); return NULL;
    } 

    for(nodeVar* tmp=l.first; tmp!=NULL; tmp=tmp->next ){
        if(stringEquals(name,tmp->name)) return tmp;
    }
    return NULL;
}

//Teste si c est bien un nom de variable correct
int isCorrectVarName(const char* c){
    //Premier carac != de nombre
    if(isdigit(*c)){
        printf("Erreur: nom de variable commenceant par un chiffre\n");
        return 0;
    }
    //doit contenir des lettres ou des nombres
    for(;*c!='\0';c++){
        if(!isalnum(c)){
            printf("Erreur: nom de variable incorrect");
            return 0;
        }
    }
    return 1;
}

//Push une variable sur l (doit commencer par une lettre) OU Met a jour une variable existante si son nom existe déjà
void varPush(const char* name, unbounded_int val, varList* l){
    
    //On vérifie les arguments
    if(l==NULL || name==NULL ||val.signe=='*'){
        printf("Erreur liste nulle, nom nulle ou nombre incorrect\n"); return;
    }

    //On vérifie que la variable ait un nom correct
    if(!isCorrectVarName(name)) return;

    //Si la variable n'existe pas, on la crée puis on l'ajoute à la liste
    nodeVar* tmp=findInList(name, *l);
    if(tmp==NULL){
        nodeVar* tmp=l->first;
        while(tmp->next!=NULL){
            tmp=tmp->next;
        }
        tmp->next=newNodeVar(name,val);
        l->len++;
    } else {
        //Sinon, on met juste à jour sa valeur
        tmp->value=val;
    }
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

//teste si c correspond à un nombre
int isNumber(const char*c){
    if(!isdigit(*c) && *c!='+' && c!='-') return 0;
    c++;
    for(; c!='\0';c++){
        if(!isdigit(*c)) return 0;
    }
    return 1;
}

//Print un message d'erreur pour print avec argument c
void printErr(char* c){
    printf("Erreur : %s",c);
}

//TODO
//Teste et print si jamais le print et correct
int isCorrectPrint(node* cur, varList* vl){
        cur=cur->next;

        //Si le jeton courant n'est ni un nombre ni une variable
        if(!isVarName(cur->token) && !isNumber(cur->token)) return 0;
        nodeVar* var=findInList(cur->token, *vl);
        if(var==NULL) return 0;

        //Si il s'agit bien de la fin de la liste alors elle est correcte
        if(cur->next==NULL) return 1;

        //Sinon on continue pour évaluer l'expression
        cur=cur->next;
        char* op=NULL;
        if(stringEquals(cur->token,"+")) op="+";
        if(stringEquals(cur->token,"-")) op="-";
        if(stringEquals(cur->token,"*")) op="*";
        if(op==NULL) return 0;

        //Si la ligne se finit par un signe alors elle est incorrecte
        if(cur->next==NULL) return 0;
        cur=cur->next;

        //On recommence la première étape
        //Si le jeton courant n'est ni un nombre ni une variable
        if(!isVarName(cur->token) && !isNumber(cur->token)) return 0;
        nodeVar* var2=findInList(cur->token, *vl);
        if(var2==NULL) return 0;

        //Si il s'agit bien de la fin de la liste alors elle est correcte
        if(cur->next==NULL) return 1;
        

        
    
}

//TODO
//Teste que la stringList l corresponde bien à une ligne CORRECTE de code
int isCorrectLine(stringList l, varList* vl){
    node* cur=l.first;
    
    //Cas print
    if(stringEquals(cur->token, "print")) return isCorrectPrint(cur, vl);

    //Cas variable
    if(isCorrectVarName(cur->token)){
        char* varName=cur->token;

        cur=cur->next;
        if(cur==NULL || !stringEquals(cur->token,"=")) return 0;

        nodeVar* var2=NULL;
        nodeVar* var3=NULL;
        unbounded_int* a=newUnbound();
        unbounded_int* b=newUnbound();

        //premier argument de l'expression
        cur=cur->next;
        if(cur==NULL || (!isCorrectVarName(cur->token) && !isNumber(cur->token))) return 0;
        //Soit une variable
        if(isCorrectVarName(cur->token)){
            free(a);
            var2=findInList(cur->token, *vl);
            if(var2==NULL) return 0;
        }
        //Soit un nombre
        if(isNumber(cur->token)){
            *a=string2unbounded_int(cur->token);
        } else return 0;
        
        //Si c'est tout, alors on attribue ça à la nouvelle variable
        cur=cur->next;
        if(cur==NULL){
            if(var2==NULL) varPush(varName,*a, vl);
            else varPush(varName, var2->value, vl);
            return 1;
        }

        //Sinon, c'est une opération
        cur=cur->next;
        if(cur==NULL) return 0;
        char* op=NULL;
        if(stringEquals(cur->token,"+")) op="+";
        if(stringEquals(cur->token,"-")) op="-";
        if(stringEquals(cur->token,"*")) op="*";
        if(op==NULL) return 0;

        //deuxième argument
        cur=cur->next;
        if(cur==NULL || (!isCorrectVarName(cur->token) && !isNumber(cur->token))) return 0;
        //Soit une variable
        if(isCorrectVarName(cur->token)){
            free(b);
            var3=findInList(cur->token, *vl);
            if(var3==NULL) return 0;
        }
        //Soit un nombre
        if(isNumber(cur->token)){
            *b=string2unbounded_int(cur->token);
        } else return 0;

        //Puis on nettoie un peu pour avoir les deux valeurs à ajouter
        if(var2==NULL && var3!=NULL){
            *b=var3->value;
        }
        if(var2!=NULL && var3==NULL){
            *a=var2->value;
        }
        if(var2!=NULL && var3!=NULL){
            *a=var2->value;
            *b=var3->value;
        }

        //Ensuite on ajoute l'éval de l'expression à la nouvelle variable
        if(stringEquals(op,"+")){
            varPush(varName,unbounded_int_somme(*a,*b),vl); return 1;
        }
        if(stringEquals(op,"-")){
            varPush(varName,unbounded_int_difference(*a,*b),vl); return 1;
        }
        if(stringEquals(op,"*")){
            varPush(varName,unbounded_int_produit(*a,*b),vl); return 1;
        } else return 0;

    }
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