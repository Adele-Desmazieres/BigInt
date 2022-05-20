#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>

#include "unbounded_int.h"

static unbounded_int err();


//FONCTIONS UTILITAIRES

//Teste si la chaîne c est un nombre
int is_number(const char* c){
	if(*c!='-' && *c!='+' && !isdigit(*c)) return 0;
	if(*c=='-' || *c=='+'){
		c++;
		if(*c=='\0') return 0;
	}
	
	for(; *c!='\0';c++){
		if(!isdigit(*c)) return 0;
	}
	return 1;
}

//Constructeur de chiffre
static chiffre* new_chiffre(char d){
	chiffre* ret=malloc(sizeof(chiffre));
	if(ret==NULL) err();
	ret->c=d;
	ret->precedent=NULL;
	ret->suivant=NULL;
	return ret;
}

//Constructeur de unbounded_int
static unbounded_int* new_unbound(){
	unbounded_int* ret=malloc(sizeof(unbounded_int));
	if(ret==NULL) err();
	ret->len=0;
	ret->signe='+';
	ret->premier=NULL;
	ret->dernier=NULL;
	return ret;
}

//Insertion d'un numéro c dans la liste l en dernière position
static void push(char c, unbounded_int* l){

	if(l==NULL) return;
	if(l->premier==NULL){ // si la liste est vide
		//Mise à jour de unbounded int
		l->premier=new_chiffre(c);
		l->dernier=l->premier;
	} else {
		chiffre* tmp=l->premier;
		while(tmp->suivant!=NULL){
			tmp=tmp->suivant;
		}
		//Mise à jour de la liste ici
		tmp->suivant=new_chiffre(c);
		l->dernier->suivant=tmp->suivant;
		l->dernier=tmp->suivant;
		l->dernier->precedent=tmp;
	}

	l->len++;
	
}

//Insertion d'un numéro c dans la liste l en première position
static void enfile(char c, unbounded_int* l){

	if(l==NULL) return;
	if(l->premier==NULL){
		//Mise à jour de unbounded int
		l->premier=new_chiffre(c);
		l->dernier=l->premier;
	} else {
		chiffre* tmp=new_chiffre(c);
		tmp->suivant=l->premier;
		l->premier->precedent=tmp;
		l->premier=tmp;
	}
	l->len++;
	
}

//Pour imprimer un chiffre
static void print_chiffre(chiffre f, int debug){
	printf("%c", f.c);
	if(debug){
		printf(" Suivant: %p Precedent: %p\n",f.suivant,f.precedent);
	}
}

//Pour imprimer un unbounded int joliement
void print_unbound(unbounded_int l, int debug){
	printf("Signe: %c   Longueur: %I64ld\n", l.signe,l.len);
	chiffre* tmp=l.premier;
	for(;tmp!=NULL; tmp=tmp->suivant){
		print_chiffre(*tmp,debug);
	}
	printf("\n");
}

//Pour transformer un long long i en chaîne de caractères
static char* ll2string(long long i){

	if(i==0) return "0";

	double log=log10(abs(i));
	int Ilen=(int)log;

	char* ret=malloc((Ilen+1)*sizeof(char));
	if(ret==NULL) err();

	//Méthode qui convertit un long long i en chaine de caractère stockée à ret, en base 10
	//lltoa(i,ret,10);
	sprintf(ret, "%lld", i);

	return ret;
}

//Fonction d'erreur, renvoie un unbounded int avec le signe *
static unbounded_int err(){
	printf("Erreur\n");

	unbounded_int* ret = malloc(sizeof(unbounded_int));
	if (ret == NULL) exit(1); // seul exit du code

	ret->signe='*';
	return *ret;
}





//FONCTIONS DU SUJET


unbounded_int string2unbounded_int( const char *e ){

	//On vérifie que la chaîne soit bien une représentation d'un nombre
	if(strlen(e)==0 || is_number(e)==0) return err();

	//On initialise un unbounded_int
	unbounded_int* ret=new_unbound();

	//On regarde le signe du nombre
	if(*e=='-'){
		ret->signe='-'; e++;
	} else if (*e=='+'){
		ret->signe='+'; e++;
	}

	//On tronque les 0 au début du nombre
	while(*e=='0' && *e!='\0' && *(e+1)!='\0'){
		e++;
	}

	//Si le nombre est vide, alors il y a une erreur
	if(*e=='\0'){
		return err();
	}

	//On insère tous les caractères du nombre dans la liste
	for(; *e!='\0';e++){
		push(*e,ret);
	}

	//On renvoie le résultat
	return *ret;
}

unbounded_int ll2unbounded_int (long long i){

	//On convertit le long long en chaîne de caractères
	char* num=ll2string(i);

	//Puis on renvoie la fonction précédente avec cette chaîne
	return string2unbounded_int(num);
}

char* unbounded_int2string(unbounded_int i){

	//On alloue la mémoire à la chaîne qu'on va renvoyer
	char* ret=malloc((i.len+1)*sizeof(char));
	if(ret==NULL) err();

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

	//Ensuite on compare les longueurs des nombres
	if(a.len>b.len) return ret;
	if(a.len<b.len) return ret2;
	
	
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
	unbounded_int c=ll2unbounded_int(b);
	return unbounded_int_cmp_unbounded_int(a,c);
}

//Déclaration de la différence
unbounded_int unbounded_int_difference( unbounded_int a, unbounded_int b);

unbounded_int unbounded_int_somme(unbounded_int a, unbounded_int b){

	//Cas généraux des signes
	if(a.signe=='-' && b.signe=='-'){
		a.signe='+'; b.signe='+';
		unbounded_int ret=unbounded_int_somme(a,b);
		if(ret.signe=='+') ret.signe='-';
		else if(ret.signe=='-') ret.signe='+';
		return ret;
	}
	if(a.signe=='+' && b.signe=='-'){
		b.signe='+';
		return unbounded_int_difference(a,b);
	}
	if(a.signe=='-' && b.signe=='+'){
		a.signe='+';
		return unbounded_int_difference(b,a);
	}

	//Ensuite le cas où les deux entiers sont positifs

	//On vérifie que les nombre ne soient pas vides
	if(a.premier==NULL || b.premier==NULL) { printf("erreur somme vide\n"); return err(); }

	//On initialise le retour
	unbounded_int* ret=new_unbound();
	if(ret==NULL) { printf("Erreur somme"); return err();} 
	ret->signe='+';

	//On itère sur les unbounded int en arrière
	chiffre *ta= a.dernier;
	chiffre *tb= b.dernier;
	int retenue=0;
	for(; ta!=NULL && tb!=NULL; ta=ta->precedent, tb=tb->precedent ){
		int resLocalGen=((ta->c-'0')+(tb->c-'0'))+retenue;
		int resLocal=resLocalGen%10;
		enfile((resLocal)+'0',ret);
		retenue=resLocalGen/10;
	}

	//Lorsqu'un nombre arrive à sa fin, alors on continue à ajouter les unités les plus grandes
	for(;ta!=NULL;ta=ta->precedent){
		int resLocalGen=(ta->c-'0')+retenue;
		int resLocal=resLocalGen%10;
		enfile((resLocal)+'0',ret);
		retenue=resLocalGen/10;
	}
	for(;tb!=NULL;tb=tb->precedent){
		int resLocalGen=(tb->c-'0')+retenue;
		int resLocal=resLocalGen%10;
		enfile((resLocal)+'0',ret);
		retenue=resLocalGen/10;
	}
	if(ta==NULL && tb==NULL && retenue!=0) enfile(retenue+'0',ret);

	//On tronque les 0 au début du nombre (en place)
	if(ret->premier->c=='0'){
		for(chiffre* c=ret->premier;c!=NULL;c=c->suivant){
			if(c->c!='0') break;
			if(c->c=='0' && c->suivant!=NULL){
				ret->premier=c->suivant;
				ret->premier->precedent=NULL;
				ret->len--;
			}
		}
	}

	return *ret;
}

unbounded_int unbounded_int_difference( unbounded_int a, unbounded_int b){

	//Cas généraux des signes
	if(a.signe=='-' && b.signe=='-'){
		a.signe='+'; b.signe='+';
		return unbounded_int_difference(b,a);
	}
	if(a.signe=='+' && b.signe=='-'){
		b.signe='+';
		return unbounded_int_somme(a,b);
	}
	if(a.signe=='-' && b.signe=='+'){
		a.signe='+';
		unbounded_int ret=unbounded_int_somme(b,a);
		if(ret.signe=='+') ret.signe='-';
		else if(ret.signe=='-') ret.signe='+';
		return ret;
	}
	//Cas où a<b
	if(unbounded_int_cmp_unbounded_int(a,b)==-1){
		unbounded_int ret=unbounded_int_difference(b,a);
		if(ret.signe=='+') ret.signe='-';
		else if(ret.signe=='-') ret.signe='+';
		return ret;
	}

	//Ensuite, le cas a>b>0

	//On vérifie que les nombre ne soient pas vides
	if(a.premier==NULL || b.premier==NULL) return err();

	
	//On initialise le retour
	unbounded_int* ret=new_unbound();
	if(ret==NULL) return err();
	ret->signe='+';

	//On itère sur les unbounded int en arrière
	chiffre *ta= a.dernier;
	chiffre *tb= b.dernier;
	int retenue=0;
	for(; ta!=NULL && tb!=NULL; ta=ta->precedent, tb=tb->precedent ){
		int resLocalGen=((ta->c-'0')-(tb->c-'0'))+retenue;
		if(resLocalGen<0){
			resLocalGen+=10;
			retenue=-1;
		} else retenue=0;
		
		enfile((resLocalGen)+'0',ret);
	}

	//Lorsqu'un nombre arrive à sa fin, alors on ajoute simplement les chiffres restants de a
	for(;ta!=NULL;ta=ta->precedent){
		int resLocalGen=(ta->c-'0')+retenue;
		if(resLocalGen<0){
			resLocalGen+=10;
			retenue=-1;
		} else retenue=0;
		enfile((resLocalGen)+'0',ret);
	}

	//On tronque les 0 au début du nombre (en place)
	if(ret->premier->c=='0'){
		for(chiffre* c=ret->premier;c!=NULL;c=c->suivant){
			if(c->c!='0') break;
			if(c->c=='0' && c->suivant!=NULL){
				ret->premier=c->suivant;
				ret->premier->precedent=NULL;
				ret->len--;
			}
		}
	}
   

	return *ret;
}

unbounded_int unbounded_int_produit( unbounded_int a, unbounded_int b){

	//On vérifie que les nombre ne soient pas vides
	if(a.premier==NULL || b.premier==NULL){
		printf("Erreur nbr vide");
		return err();
	}

	//Si un des deux nombres est égal à 0, on renvoie simplement 0
	if(a.premier->c=='0' || b.premier->c=='0'){
		return string2unbounded_int("0");
	} 

	//On intialise le retour
	unbounded_int* ret=new_unbound();
	if(ret==NULL){
		printf("Erreur ret");  
		return err();
	} 
	push('0',ret);

	//Ensuite on itère sur b
	int dec=0;
	for(chiffre *tb=b.dernier ; tb!=NULL; tb=tb->precedent){

		//On initialise le nombre temporaire dans la multiplication
		unbounded_int* tmp=new_unbound();
		if(tmp==NULL){
			printf("Erreur tmp");
			return err();
		} 

		//On enfile 'dec' 0 de decalage
		for(int i=0;i<dec;i++){
			enfile('0',tmp);
		}

		//Puis on itère sur a
		int retenue=0;
		for(chiffre *ta=a.dernier; ta!=NULL; ta=ta->precedent){
			int resLocalGen=retenue+(ta->c-'0')*(tb->c-'0');
			int resLocal=resLocalGen%10;
			enfile((resLocal+'0'),tmp);
			retenue=resLocalGen/10;
		}
		
		//Ensuite on enfile la retenue
		enfile((retenue+'0'),tmp);
		
		//On incrémente le décalage
		dec++;

		//Et on ajoute le resultat au nombre de retour
		*ret=unbounded_int_somme(*ret,*tmp);
		free(tmp);
	}

	//Le signe est enfin déterminé par a et b
	if(a.signe==b.signe) ret->signe='+';
	else ret->signe='-';

	//On tronque les 0 au début du nombre (en place)
	if(ret->premier->c=='0'){
		for(chiffre* c=ret->premier;c!=NULL;c=c->suivant){
			if(c->c!='0') break;
			if(c->c=='0' && c->suivant!=NULL){
				ret->premier=c->suivant;
				ret->premier->precedent=NULL;
				ret->len--;
			}
		}
	}

	return *ret;
}