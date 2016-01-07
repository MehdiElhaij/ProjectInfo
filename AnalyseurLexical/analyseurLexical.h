#ifndef _ANALYSEUR_LEXICAL_H
#define _ANALYSEUR_LEXICAL_H


#define VRAI 1
#define FAUX 0


typedef struct
{
	char valeur[50];
	int type;
	int debut, // information sur lq position du lexeme
		fin,
		line;
}LEXLEME;


struct _LEXEME_TYPE
{
	int motCle,
		mot,
		ponctuateur,
		nombreEntier,
		nombreDecimale,
		chaineDeChars,
		commentaire,
		boolean,
		operateur_boolean,

		// les operateur entre 20 et 29
		plus_moin,
		mult_divi,
		affectation,
		// les operatuer boolean entre 30 et 31
		operatuer_egualite,
		operatuer_difference,
		operatuer_ET,
		operatuer_OU,
		operatuer_sup,
		operatuer_inf,
		operatuer_negation,
        
        // > 50
        motCle_ALGORITHME,
		motCle_DEBUT,
		motCle_VARIABLE,
		motCle_CONSTANT,
		motCle_FONCTION,
		// les intruction de controle
		motCle_SI,
		motCle_SINON,
		motCle_SINONSI,
		motCle_ALORS,
		motCle_FINSI,

		// les boucle
		motCle_REPETER,
		motCle_JUSQUA,

		// les type 
		motCle_ENTIER,
		motCle_REEL,
		motCle_BOOL,

		// des fonctions special on a considere comme des mot-cle
		motCle_LIRE,
		motCle_ECRIRE,
        
		motCle_FIN,

		finLine,
		FIN;
};



int est_ce_que_debut_de_un_mot(char c);
int est_ce_que_part_de_un_mot(char c);
int est_ce_que_un_espace(char c);
int chaine_comparaison(char const* , char const*);

int obtenir_char();
void sauter_les_espace();

LEXLEME obtenir_nombre();
LEXLEME obtenir_ponctuateur();

int est_ce_que_mot_cle(char*);
LEXLEME obtenir_mot();
LEXLEME obtenir_lexeme();


void init(char*);

// Global Vars
extern struct _LEXEME_TYPE LEXEME_TYPE; 


extern char source[]; // va etre remplie
extern int index_limite; // c'est nbr de chars dans source
extern char ch;
extern int index;
extern int line_index;


#endif