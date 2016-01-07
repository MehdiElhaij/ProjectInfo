#include "analyseurLexical.h"



struct _LEXEME_TYPE LEXEME_TYPE = {
		.motCle = 0,
		.mot = 1,
		.ponctuateur = 2,
		.nombreEntier = 3,
		.nombreDecimale = 4,
		.chaineDeChars = 5,
		.commentaire = 6,
		.boolean = 7,
		.operateur_boolean = 8,

		// les operateur entre 20 et 29
		.plus_moin = 20,
		.mult_divi = 21,
		.affectation = 22,
		// les operatuer boolean entre 30 et 31
		.operatuer_egualite = 30,
		.operatuer_difference = 31,
		.operatuer_ET = 32,
		.operatuer_OU = 33,
		.operatuer_sup = 34,
		.operatuer_inf = 35,
		.operatuer_negation = 36,

        .motCle_ALGORITHME = 51,
		.motCle_DEBUT = 52,
		.motCle_VARIABLE = 53,
		.motCle_CONSTANT = 54,
		.motCle_FONCTION = 55,
		// les intruction de controle
		.motCle_SI = 56,
		.motCle_SINON = 57,
		.motCle_SINONSI = 58,
		.motCle_ALORS = 59,
		.motCle_FINSI = 60,

		// les boucle
		.motCle_REPETER = 61,
		.motCle_JUSQUA = 62,

		// les type 
		.motCle_ENTIER = 70,
		.motCle_REEL = 71,
		.motCle_BOOL = 73,

		// des foonctions special on a considere comme des mot-cle
		.motCle_LIRE = 90,
		.motCle_ECRIRE = 91,

		.motCle_FIN = 89,
        
        
		.finLine = 98,
		.FIN = 99,
};



char source[1024]; // va etre remplie
int index_limite; // c'est nbr de chars dans source
char ch;
int index;
int line_index;



// des fonction boolean 1: vrais 0: faux
int est_ce_que_debut_de_un_mot(char c){
	if(isalpha(c) || c == '_')
		return VRAI;
	return FAUX;
}

int est_ce_que_part_de_un_mot(char c){
	if(isalpha(c) || isdigit(c) || c == '_')
		return VRAI;
	return FAUX;
}

int est_ce_que_un_espace(char c){
	if(c == ' ' || c == '\s' || c == '\t')
		return VRAI;
	return FAUX;
}

// determiner si deux chaine sont egaux
// returner 0 si vrais
int chaine_comparaison(char const *a, char const *b)
{
    for (;; a++, b++) {
        int d = tolower(*a) - tolower(*b);
        if (d != 0 || !*a)
            return d;
    }
}


/////////////////////////////////////////////
//////Analyseur Lexical
/////////////////////////////////////////////

// Declaration Global 
LEXLEME lexemes[5000]; // tableau des lexlemes
LEXLEME lexeme;



// chercher la caractere suivant sinon returner FAUX
int obtenir_char(){
	if(index <= index_limite ){
		ch = source[index];
		index++;
		return VRAI;
	}
	return FAUX;
}

void sauter_les_espace(){
	while(est_ce_que_un_espace(ch) == VRAI && obtenir_char() == VRAI)
	{
		// rien a faire ici
	}
}

LEXLEME obtenir_nombre(){
	LEXLEME lexeme;

	lexeme.type = LEXEME_TYPE.nombreEntier;
	lexeme.debut = index;
	lexeme.line = line_index;
    
    int char_index = 0;
	do{
		lexeme.valeur[char_index] = ch;
		char_index++;
	}while(obtenir_char() && isdigit(ch));
	

	// les nombres decimals
	if(ch == '.'){
		// verifier si le premier char est un nombre
		do{
			lexeme.valeur[char_index] = ch;
			char_index++;
		}while(obtenir_char() && isdigit(ch));
		lexeme.type = LEXEME_TYPE.nombreDecimale;
	}
	lexeme.valeur[char_index] = '\0'; //fermer la chaine de chars
    lexeme.fin = index;
	return lexeme;
}

LEXLEME obtenir_ponctuateur(){
	LEXLEME lexeme;

	lexeme.type = LEXEME_TYPE.ponctuateur;
	lexeme.debut = index;
	lexeme.line = line_index;
    
    lexeme.valeur[0] = ch;
    lexeme.valeur[1] = '\0'; // va etre modifier si operateur est complex
	lexeme.valeur[2] = '\0'; // fermer la chaine si l'operateur est complex
    obtenir_char();


    // # + -
	if(ch == "#")
		lexeme.type = LEXEME_TYPE.operatuer_difference;
	else if(ch == '+' || ch == '-')
		lexeme.type = LEXEME_TYPE.plus_moin;
	else if(ch == '*' || ch == '/')
		lexeme.type = LEXEME_TYPE.mult_divi;


	// est ce que : || && ==
	if( lexeme.valeur[0] == ch &&
		(ch == '=' || ch == '&' || ch == '|') )
	{
		lexeme.valeur[1] = ch;
		if(ch == '=') lexeme.type = LEXEME_TYPE.operatuer_egualite;
		else if(ch == '&') lexeme.type = LEXEME_TYPE.operatuer_ET;
		else if(ch == '|') lexeme.type = LEXEME_TYPE.operatuer_OU;
		obtenir_char();
	}

	// :=
	if( lexeme.valeur[0] == ':' && ch == '=' )
	{
		lexeme.valeur[1] = ch;
		lexeme.type = LEXEME_TYPE.affectation;
		obtenir_char();
	}

	// < <= <-
	if( lexeme.valeur[0] == '<'){
		lexeme.type = LEXEME_TYPE.operatuer_sup;
		if(ch == '='){
			lexeme.valeur[1] = ch;
			obtenir_char();
		}
		else if(ch == '-'){
			lexeme.valeur[1] = ch;
			lexeme.type = LEXEME_TYPE.affectation;
			obtenir_char();
		}
	}

	// > >=
	if( lexeme.valeur[0] == '>'){
		lexeme.type = LEXEME_TYPE.operatuer_sup;
		if(ch == '=')
			obtenir_char();
	}

	// ! !=
	if( lexeme.valeur[0] == '!'){
		lexeme.type = LEXEME_TYPE.operatuer_negation;
		if(ch == '='){
			lexeme.type = LEXEME_TYPE.operatuer_difference;
			obtenir_char();
		}
	}

	lexeme.fin = index;
	return lexeme;

}

// determiner si un mot est un Mot-cle
// returner la valeur correspondant d'apres lq structure 'MOT_CLE'
// sinon returner FAUX 
int est_ce_que_mot_cle(char* mot){
	if(chaine_comparaison(mot, "ALGORITHME") == 0)
		return LEXEME_TYPE.motCle_ALGORITHME;
	else if(chaine_comparaison(mot, "DEBUT") == 0)
		return LEXEME_TYPE.motCle_DEBUT;
	else if(chaine_comparaison(mot, "VAR") == 0 || 
			chaine_comparaison(mot, "VARIABLE") == 0 ||
			chaine_comparaison(mot, "VARIABLES") == 0)
		return LEXEME_TYPE.motCle_VARIABLE;
	else if(chaine_comparaison(mot, "CONST") == 0 || 
			chaine_comparaison(mot, "CONSTANT") == 0 ||
			chaine_comparaison(mot, "CONSTANTS") == 0)
		return LEXEME_TYPE.motCle_CONSTANT;
	else if(chaine_comparaison(mot, "FONCTION") == 0)
		return LEXEME_TYPE.motCle_FONCTION;
	else if(chaine_comparaison(mot, "SI") == 0)
		return LEXEME_TYPE.motCle_SI;
	else if(chaine_comparaison(mot, "SINON") == 0)
		return LEXEME_TYPE.motCle_SINON;
	else if(chaine_comparaison(mot, "SINONSI") == 0)
		return LEXEME_TYPE.motCle_SINONSI;
	else if(chaine_comparaison(mot, "ALORS") == 0)
		return LEXEME_TYPE.motCle_ALORS;
	else if(chaine_comparaison(mot, "FINSI") == 0)
		return LEXEME_TYPE.motCle_FINSI;
	else if(chaine_comparaison(mot, "REPETER") == 0)
		return LEXEME_TYPE.motCle_REPETER;
	else if(chaine_comparaison(mot, "JUSQUA") == 0)
		return LEXEME_TYPE.motCle_JUSQUA;
	else if(chaine_comparaison(mot, "ENTIER") == 0)
		return LEXEME_TYPE.motCle_ENTIER;
	else if(chaine_comparaison(mot, "REEL") == 0)
		return LEXEME_TYPE.motCle_REEL;
	else if(chaine_comparaison(mot, "BOOL") == 0)
		return LEXEME_TYPE.motCle_BOOL;
	else if(chaine_comparaison(mot, "LIRE") == 0)
		return LEXEME_TYPE.motCle_LIRE;
	else if(chaine_comparaison(mot, "ECRIRE") == 0)
		return LEXEME_TYPE.motCle_ECRIRE;
	else if(chaine_comparaison(mot, "FIN") == 0)
		return LEXEME_TYPE.motCle_FIN;
	else
		return FAUX;
}

LEXLEME obtenir_mot(){
	LEXLEME lexeme;

	lexeme.type = LEXEME_TYPE.mot;
	lexeme.debut = index;
	lexeme.line = line_index;

	int char_index = 0;
	do{
		lexeme.valeur[char_index] = ch;
		char_index++;
	}while(obtenir_char() && est_ce_que_part_de_un_mot(ch));
	lexeme.valeur[char_index] = '\0'; //fermer la chaine de chars

	lexeme.fin = index;
	int type = est_ce_que_mot_cle(lexeme.valeur);
	if(type != FAUX)
		lexeme.type = type;
	return lexeme;
}

LEXLEME obtenir_lexeme(){
	LEXLEME lexeme;
    lexeme.type = LEXEME_TYPE.FIN;
	lexeme.debut = index;
	if(ch == '\n'){
		lexeme.type = LEXEME_TYPE.finLine;
		lexeme.fin = index+1;
		line_index++;
		
        obtenir_char();
		return lexeme;
	}

	if(est_ce_que_debut_de_un_mot(ch) == VRAI)
		lexeme = obtenir_mot();
    else if(isdigit(ch) != FAUX)
        lexeme = obtenir_nombre();
    else if(ch == '\0')
        return lexeme;
    else
        lexeme = obtenir_ponctuateur();
    
    sauter_les_espace();
    
    return lexeme;
	
}




void init(char* input){
    
    strcpy(source, input);
    
    index_limite = strlen(source);
    index = 0;
    line_index = 0;
    
    // debut
    obtenir_char();
}