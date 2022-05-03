#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <assert.h>

#include "unbounded_int.c"


// 888b     d888                                 d8b                 
// 8888b   d8888                                 Y8P                 
// 88888b.d88888                                                     
// 888Y88888P888  .d88b.  88888b.d88b.   .d88b.  888 888d888 .d88b.  
// 888 Y888P 888 d8P  Y8b 888 "888 "88b d88""88b 888 888P"  d8P  Y8b 
// 888  Y8P  888 88888888 888  888  888 888  888 888 888    88888888 
// 888   "   888 Y8b.     888  888  888 Y88..88P 888 888    Y8b.     
// 888       888  "Y8888  888  888  888  "Y88P"  888 888     "Y8888  


// listes de variables

// nodeVar se référence lui-même 
typedef struct nodeVar nodeVar;

//Structure noeud de varList
typedef struct nodeVar {
    unbounded_int value;
    char* name;
    nodeVar* next;
} nodeVar;

//Structure varList
typedef struct varList{
    size_t len;
    nodeVar* first;
} varList;


//Constructeur nodeVar avec unbounded int
nodeVar* new_nodeVar(const char* c, unbounded_int val){

    //On init. le noeud
    nodeVar* ret=malloc(sizeof(nodeVar));
    if (ret==NULL){
        printf("Erreur constructeur nodeVar\n");
        exit(1);
    }
       
    ret->value=val;

    //On copie la valeur de c
    ret->name=malloc((strlen(c)+1)*sizeof(char));
    if(ret->name==NULL){
        printf("Erreur malloc sur copie de str pour newNodeVar\n"); 
        exit(1);
    }
    strcpy(ret->name,c);

    ret->next=NULL;

    return ret;
}

//Constructeur de varList
varList* new_varList(){

    //Init. val de retour
    varList* ret=malloc(sizeof(varList));
    if (ret==NULL){
        printf("Erreur constructeur varList\n");
        exit(1);
    }

    ret->len=0;
    ret->first=NULL;

    return ret;
}


//Affiche la liste des variables
void print_varList(varList vl){
    printf("Taille : %ld\n", vl.len);
    for(nodeVar* tmp=vl.first; tmp!=NULL; tmp=tmp->next){
        printf("Variable : %s\n", tmp->name);
        printf("Valeur : ");
        print_unbound(tmp->value, 0);
    }
}


//Trouve et renvoie un nodeVar de nom name si il existe dans la liste l, renvoie NULL sinon
nodeVar* finds_in(varList l, const char* name){
    if (name==NULL){
        //printf("Erreur nom nul\n"); 
        return NULL;
    } 

    for (nodeVar* tmp=l.first; tmp!=NULL; tmp=tmp->next ){
        if (strcmp(name,tmp->name) == 0) return tmp;
    }
    return NULL;
}



// teste si c est bien un nom de variable correct
// c'est-a-dire commencant par une lettre et contenant que des lettres et chiffres
// et différent des mots-clefs de l'interpreteur
int is_var_name(const char* c){
    // premier caractere doit etre une lettre
    if (!isalpha(*c)){
        return 0;
    }
    
    // doit contenir que des lettres ou des chiffres
    for (;*c!='\0'; c++){
        if (!isalnum(*c)){
            return 0;
        }
    }
    
    // doit etre different des mots clefs de l'interpreteur
    char motsclefs[] = {"print"};
    int nbr_motsclefs = 1;
    for (int i = 0; i < nbr_motsclefs; i++) {
        char mot = motsclefs[i];
        if (strcmp(&mot, c) == 0) return 0;    
    }
    
    return 1;
}



//Push une variable sur l (doit commencer par une lettre) OU Met a jour une variable existante si son nom existe déjà
void var_push(const char* name, unbounded_int val, varList* l){
    
    //On vérifie les arguments
    if(l==NULL || name==NULL ||val.signe=='*'){
        printf("Erreur liste nulle, nom nul ou nombre incorrect\n"); 
        return;
    }

    //On vérifie que la variable ait un nom correct
    if(!is_var_name(name)){
        //printf("Raté!\n");
        return;
    } 

    nodeVar* tmp=finds_in(*l, name);
    //printf("%s NULL? %d\n", name,tmp==NULL);

    if (tmp != NULL) {
        // la variable existe, on met à jour sa valeur
        tmp->value=val;
        
    } else {
    // sinon la variable n'existe pas, on la crée puis on l'ajoute à la liste
            
        // tete de liste si vide
        nodeVar* prec=l->first;
        if(prec == NULL){ l->first=new_nodeVar(name,val); l->len++; return; }
        
        // parcours de la liste sinon
        while(prec->next!=NULL){
            prec = prec->next;
        }
        //printf("Par ici 2!\n");
        
        prec->next=new_nodeVar(name,val);
        l->len++;
    }
}



//        d8888          888                                   888            
//       d88888          888                                   888            
//      d88P888          888                                   888            
//     d88P 888 888  888 888888 .d88b.  88888b.d88b.   8888b.  888888 .d88b.  
//    d88P  888 888  888 888   d88""88b 888 "888 "88b     "88b 888   d8P  Y8b 
//   d88P   888 888  888 888   888  888 888  888  888 .d888888 888   88888888 
//  d8888888888 Y88b 888 Y88b. Y88..88P 888  888  888 888  888 Y88b. Y8b.     
// d88P     888  "Y88888  "Y888 "Y88P"  888  888  888 "Y888888  "Y888 "Y8888  



//Code pour les listes de string

//Structure noeud de stringList
// un entier, une variable, un mot-clef, ou un opérateur
typedef struct node node;
typedef struct node{
    char* token;
    node* next;
} node;

//Structure stringList
typedef struct{
    size_t len;
    node* first;
} stringList;

//Constructeur liste de Node
node* new_node(const char* c, int len){

    //Si la taille est nulle alors pb
    if(len==0) {
        printf("Erreur chaine nulle\n");
        return NULL;
    }

    //On forme la chaine de caractère depuis la ligne
    char* tok= malloc((len+1)*sizeof(char));
    for(int i=0;i<len;i++){
        tok[i]=c[i];
    }
    tok[len]='\0';

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
stringList* new_stringList(){

    stringList* ret=malloc(sizeof(stringList));
    if (ret==NULL){
        printf("Erreur constructeur stringList\n");
        exit(1);
    }

    ret->len=0;
    ret->first=NULL;

    return ret;
}


//Push une chaine de caractère c sur l
void string_push(const char* c, int len, stringList* l){
    node* noeud=new_node(c, len);

    if(l->first==NULL){
        l->first=noeud;
    } else {
        node* tmp=l->first;
        for(; tmp->next!=NULL; tmp=tmp->next){
        }
        tmp->next=noeud;
    }

    l->len++;
}


// On transforme une ligne renvoyée par fgets() en stringList d'arguments
// sépare les caractères en tokens
stringList line2stringList(const char* rawLine){

    //On intitialise le retour
    stringList* ret=new_stringList();

    //On parcourt la ligne jusqu'à la fin (bon caractère?)
    while(*rawLine!='\n' && *rawLine!='\0'){
        
        if(*rawLine!=' ' && *rawLine!='='){
            //On détermine la longueur de la chaine
            int len=0;

            //On créé une copie de rawLine dans tmp
            char* tmp=malloc((strlen(rawLine)+1)*sizeof(char));
            strcpy(tmp,rawLine);
            
            // calcul de la longueur du token suivant
            while(*tmp!=' ' && *tmp!='\n' && *tmp!='=' && *tmp!='\0'){
                len++;
                tmp++;
            }

            //On ajoute la chaine à la liste
            string_push(rawLine, len, ret);

            //On skip la chaine trouvée pour éviter de créer des immondices
            rawLine=rawLine+len;
        }

        rawLine++;
    }

    return *ret;
}


char* stringList2string(stringList l){

    //On cherche la longueur du string de retour
    int totalLen=1;
    for(node* i=l.first; i!=NULL; i=i->next){
        totalLen+=strlen(i->token)+1;
    }

    //Init. du retour
    char* ret=malloc(sizeof(char)*totalLen);
    if(ret==NULL){
        printf("Erreur list2string malloc\n");
        exit(1);
    }
    int retIndex=0;

    //Construction du retour
    for(node* i=l.first; i!=NULL; i=i->next){
        for(char* tmp=i->token;*tmp!='\0';tmp++){
            ret[retIndex++]=*tmp;
        }
        ret[retIndex++]=' ';
    }
    ret[retIndex++]='\n';
    ret[retIndex]='\0';

    return ret;
}


void print_stringList(stringList l){
    for(node* i=l.first; i!=NULL; i=i->next){
        printf("%s ", i->token);
    }
    printf("\n");
}


// vérifie que l'opération est correcte
// c'est-a-dire composée d'un nbr ou var, puis un opérateur, puis nbr ou var
// avec des var initialisées
int node_is_operation(node* cur, varList* vl) {
    int i = 0;
    
    for (node* tmp = cur; tmp != NULL; tmp = tmp->next) {
        char* nom = tmp->token;
        
        // le cas des operandes (variable initialisée ou nombre)
        if (i % 2 == 0) {
            if (!is_number(nom) && !(is_var_name(nom) && finds_in(*vl, nom))) return 0;
            
        // le cas des opérateurs
        } else {
            int is_operateur = 0;
            char operateurs[3] = {'+', '-', '*'};
            int nbr_operateurs = 3;
            for (int i = 0; i < nbr_operateurs; i++) {
                char operateur_string[2] = {operateurs[i] , '\0'};
                if (strcmp(operateur_string, nom) == 0) is_operateur = 1; 
            }
            if (is_operateur == 0) return 0;
        }
        
        if (i > 2) return 0; // arrete la boucle si trop d'éléments
        i++;
    }
    
    // vérifie qu'il n'y a exactement 3 éléments dans l'opération
    return (i == 3);
}


// renvoie l'unbounded_int correspondant au nombre ou à la variable contenue dans le noeud cur
unbounded_int* evaluer_operande(node* cur, varList* vl) {
    unbounded_int* ret = new_unbound();
    if (is_number(cur->token)) *ret = string2unbounded_int(cur->token);
    else {
        nodeVar* tmp = finds_in(*vl, cur->token);
        *ret = tmp->value;
    }
    return ret;
}

// renvoie l'unbouded_int correspondant à l'évaluation d'une opération avec 2 opérandes
unbounded_int evaluer_operation(node* cur, varList* vl) {
    node* tmp = cur;
    
    unbounded_int operande1 = *evaluer_operande(tmp, vl);
    tmp = tmp->next;
    
    char operateur = tmp->token[0];
    tmp = tmp->next;
        
    unbounded_int operande2 = *evaluer_operande(tmp, vl);
    
    if (operateur == '+') return unbounded_int_somme(operande1, operande2);
    else if (operateur == '-') return unbounded_int_difference(operande1, operande2);
    else return unbounded_int_produit(operande1, operande2);
}


// evalue l'expression stockée par cur, et conserve sa valeur dans resultat
// si flag = 1 alors une variable non-init est évaluée à 0
// renvoie 1 en cas de réussite et 0 en cas d'échec 
int evaluer_expression(unbounded_int *resultat, node *cur, varList *vl, int flag) {
    
    // vérifie si c'est une opération à droite du =
    if (node_is_operation(cur, vl) == 1) {
        //printf("opération\n");
        // sinon l'attribution est réussie
        *resultat = evaluer_operation(cur, vl);
        return 1;
    
    // vérifie si c'est un nombre
    } else if (is_number(cur->token) && cur->next == NULL) {
        //printf("nombre\n");
        *resultat = string2unbounded_int(cur->token);
        return 1;
        
    // vérifie si c'est une variable
    } else if (is_var_name(cur->token) && cur->next == NULL) {
        //printf("variable\n");
        
        // vérifie que la variable est initialisée
        nodeVar *tmp = new_nodeVar("", *new_unbound());
        tmp = finds_in(*vl, cur->token);
        
        // si la variable est non initialisée
        if (tmp == NULL) { 
            //printf("non-initilisée\n");
            // elle vaut 0 si le flag est levé (=1)
            if (flag == 1) {
                *resultat = string2unbounded_int("0");
                return 1;
            } 
            // sinon c'est une erreur
            else return 0;
        }
        
        // sinon on conserve la valeur de la variable trouvée
        *resultat = tmp->value; 
        return 1;    
    } 
    
    else return 0;
    
}


//Print une variable dans le flot local choisi
void print_var_in_flot(nodeVar* node, FILE* flot){
    if(flot==NULL || flot==stdout) {
        printf("%s\n", unbounded_int2string(node->value));
    } else {
        fputs(unbounded_int2string(node->value),flot);
        fputc('\n',flot);
    }
}


//Print un unbounded int dans le flot local choisi
void print_unbounded_in_flot(unbounded_int ui, FILE* flot){
    if(flot==NULL || flot==stdout) {
        printf("%s\n", unbounded_int2string(ui));
    } else {
        fputs(unbounded_int2string(ui),flot);
        fputc('\n',flot);
    }
}


// execute une commande d'attribution de variable
// qui commence forcément par un nom de variable 
// renvoie 1 si réussie, et 0 sinon
int run_var_attribution(node *cur, varList *vl) {
    
    char *var = cur->token; // nom de la variable à attribuer
    //if (!is_var_name(var)) return 0; // vérifié avant l'appel
    cur = cur->next;
    if (cur == NULL) return 0;
        
    /* A REMETTRE QUAND IL Y AURA LES TOKENS "="
    // erreur si la variable n'est pas suivi d'un égal
    if (strcmp(cur->token, "=") != 0) {
        //printf("%s", cur->token);
        return 0;
    } 
    cur = cur->next;
    if (cur == NULL) return 0;
    */
    
    unbounded_int* resultat = new_unbound();
    
    int evaluation_reussie = evaluer_expression(resultat, cur, vl, 0);
    if (evaluation_reussie == 0) return 0;
    
    var_push(var, *resultat, vl); // push la variable dans la mémoire
    
    return 1;
    
}


// vérifie que la liste de token correspond à une commande print valide
// c'est-a-dire "print" suivi par un nombre, ou une variable, ou une opération valide
// puis affiche le résultat de la commande
// renvoie 0 en cas d'erreur
int run_print(node* cur, varList* vl, FILE* flot) {
    if (cur->next == NULL) return 0;
    
    cur = cur->next;
    unbounded_int *resultat = new_unbound();
    
    int evaluation_reussie = evaluer_expression(resultat, cur, vl, 1);
    if (evaluation_reussie == 0) return 0;
    
    print_unbounded_in_flot(*resultat, flot);
    return 1;
}



// Teste que la stringList l corresponde bien à une ligne CORRECTE de code
// et exécute cette line
int run_line(stringList l, varList* vl, FILE* flot){
    node* cur=l.first;
    
    //Cas print
    if(strcmp(cur->token, "print") == 0){
        return run_print(cur, vl, flot);
    } 

    //Cas variable
    if(is_var_name(cur->token)){
        //printf("true");
        return run_var_attribution(cur, vl);
    }
    
    //Autre cas incorrect
    return 0;
}




// 888b     d888          d8b          
// 8888b   d8888          Y8P          
// 88888b.d88888                       
// 888Y88888P888  8888b.  888 88888b.  
// 888 Y888P 888     "88b 888 888 "88b 
// 888  Y8P  888 .d888888 888 888  888 
// 888   "   888 888  888 888 888  888 
// 888       888 "Y888888 888 888  888 


int main(int argc, char **argv){

    varList* mem=new_varList();

    FILE* output = NULL;
    FILE* input = stdin;

    //Lecture des arguments
    int i=1;
    for(char* tmp=argv[i]; i<argc; i++, tmp=argv[i]){
        //Selecteur de fichier de sortie
        if(strcmp(tmp,"-o") == 0 && i+1<argc){
            output=fopen(argv[i+1],"w");
            if(output == NULL) perror("Fichier de sortie incorrect"); 
        }
        //Selecteur de fichier d'entrée
        if(strcmp(tmp,"-i") == 0 && i+1<argc){
            input=fopen(argv[i+1],"r");
            if(input == NULL) perror("Fichier d'entrée incorrect"); 
        }
    }



    //Lecture de l'entrée
    char* BUF = malloc(2049*sizeof(char));
    assert(BUF != NULL);
    BUF = fgets(BUF,2048,input);
    while(BUF != NULL) {
        stringList tmpStrList = line2stringList(BUF);
        if(!run_line(tmpStrList, mem, output)){
            printf("Ligne incorrecte\n");
        }
        BUF = fgets(BUF,2048,input);
    }

    fclose(input);
    fclose(output);
    



 

    // 88888888888                888             
    //     888                    888             
    //     888                    888             
    //     888   .d88b.  .d8888b  888888 .d8888b  
    //     888  d8P  Y8b 88K      888    88K      
    //     888  88888888 "Y8888b. 888    "Y8888b. 
    //     888  Y8b.          X88 Y88b.       X88 
    //     888   "Y8888   88888P'  "Y888  88888P' 


    /*
    char* testLine2String="Test = 3 * 4\n";
    stringList test1=line2stringList(testLine2String);
    print_stringList(test1);


    printf("Est une ligne correcte: %d\n", run_line(test1, mem, NULL));


    char* testLine2String2="Test = Test * 3\n";
    stringList test2=line2stringList(testLine2String2);
    print_stringList(test2);

    printf("Est une ligne correcte: %d\n", run_line(test2, mem, NULL));

    //printVarList(*mem);
    */

    /*
    char* testLine2String3="A = 2 * 85\n";
    stringList test3=line2stringList(testLine2String3);
    print_stringList(test3);

    printf("Est une ligne correcte: %d\n", run_line(test3, mem, NULL));

    //printUnbound(finds_in(*mem, "Test")->value, 0);

    char* testLine2String4="print A * A\n";
    stringList test4=line2stringList(testLine2String4);
    print_stringList(test4);

    printf("Est une ligne correcte: %d\n", run_line(test4, mem, NULL));

    printVarList(*mem);
    */

}