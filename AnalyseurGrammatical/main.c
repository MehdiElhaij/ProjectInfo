#include <stdio.h>
#include <windows.h>
#include "../AnalyseurLexical/analyseurLexical.h"

#define ANSI_COLOR_RED     "\x1b[0m"
#define ANSI_COLOR_GREEN   "\x1b[0m"
#define ANSI_COLOR_YELLOW  "\x1b[0m"
#define ANSI_COLOR_BLUE    "\x1b[0m"
#define ANSI_COLOR_MAGENTA "\x1b[0m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

LEXLEME lexeme;

void EmitLn(char* msg){
    printf("\t%s\n",msg);
}

void erreur(){
    SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_RED |
                                                            FOREGROUND_GREEN | 
                                                            FOREGROUND_INTENSITY | 
                                                            BACKGROUND_RED);
    printf(ANSI_COLOR_RED"Erreur position %d:%d dans '%s',  \n"ANSI_COLOR_RESET,lexeme.debut,lexeme.fin,lexeme.valeur);
}

void quiter(){
    system("pause");
    exit(0);
}

int expect(int type){
    lexeme = obtenir_lexeme();
    if(lexeme.type == type)
        return  VRAI;

    return FAUX;
}

int match(char* valeur){
	if(strcmp(lexeme.valeur,valeur) == 0){
        lexeme = obtenir_lexeme();
        return VRAI;
    }
    
    return FAUX;
}

void affectation(){
    
    if(expect(LEXEME_TYPE.mot) != VRAI){
        erreur();
        printf(ANSI_COLOR_RED "if faux donner le nom du variable\n"ANSI_COLOR_RESET);
        quiter();
    }
    
    char nom_du_variable[50];
    
    strcpy(nom_du_variable,lexeme.valeur);
    
    if(expect(LEXEME_TYPE.ponctuateur) != VRAI){
        erreur();
        printf(ANSI_COLOR_RED "if faut utilizer un operateur d'affectation ici\n"ANSI_COLOR_RESET);
        quiter();
    }
    
     if(strcmp(lexeme.valeur,"=") != 0){
        erreur();
        printf(ANSI_COLOR_RED "if faut utilizer '=' comme operator d'affectation\n"ANSI_COLOR_RESET);
        quiter();
    }
        

    lexeme = obtenir_lexeme();
    expression();
    
    
    if(match(";") != VRAI){
        erreur();
        printf(ANSI_COLOR_RED "tout instruction doit etre fini par ';'\n"ANSI_COLOR_RESET);
        quiter();
    }
    
    printf("\tLEA %s(PC),A0\n",nom_du_variable);
    EmitLn("MOVE D0,(A0)");
    
}

void addition(){
	lexeme = obtenir_lexeme();
	factor();
	EmitLn("ADD (SP)+,D0");
}

void substraction(){
	//match("-");
	lexeme = obtenir_lexeme();
    factor();
	
    EmitLn("SUB (SP)+,D0");
	EmitLn("NEG D0");
}

void multiplication(){
	//match("*");
	lexeme = obtenir_lexeme();
    factor();
	EmitLn("MULS (SP)+,D0");
}

void division(){
	//match("/");
	lexeme = obtenir_lexeme();
    factor();
	EmitLn("DIVS (SP)+,D1");
	EmitLn("DIVS D1,D0");
}


// BNF: <factor> ::=  <number> | '(' <expression> ')'
void factor(){
	if( strcmp(lexeme.valeur,"(") == 0 ){
		lexeme = obtenir_lexeme();
		expression();
        if(match(")") != VRAI){
            erreur();
            printf(ANSI_COLOR_RED "if faux ferme l'expression avec '('\n"ANSI_COLOR_RESET);
            quiter();
        }
	}else if(lexeme.type == LEXEME_TYPE.nombreDecimale || lexeme.type == LEXEME_TYPE.nombreEntier){
		printf("\tMOVE #%s, D0\n",lexeme.valeur);
        lexeme = obtenir_lexeme();
	}else{
        erreur();
        printf(ANSI_COLOR_RED "'%s' ne doit pas etre ici\n"ANSI_COLOR_RESET,lexeme.valeur);
        quiter();
    }
}

//BNF: <digit> := [0-9]
//BNF: <term> := <factor> [ <mulop> <factor> ]*
void term(){
	factor();
	while(strcmp(lexeme.valeur,"*")  == 0 || strcmp(lexeme.valeur,"/") == 0){
		EmitLn("MOVE D0,-(SP)");
		if(strcmp(lexeme.valeur,"*") == 0)
			multiplication();
		else if(strcmp(lexeme.valeur,"/") == 0)
			division();
	}
}

//Expression
//BNF: <expression> := <term> '+' (<expression> | <term>) 
void expression(){
	term();

	while(strcmp(lexeme.valeur,"+") == 0 || strcmp(lexeme.valeur,"-") == 0){
        EmitLn("MOVE D0,-(SP)");
		if(strcmp(lexeme.valeur,"+") == 0)
			addition();
		else if(strcmp(lexeme.valeur,"-") == 0)
			substraction();
	}
		
}


int main(int argc, char **argv)
{
    SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED |
                                                            FOREGROUND_GREEN | 
                                                            FOREGROUND_INTENSITY | 
                                                            BACKGROUND_BLUE
                          );
    printf("Analyseur Grammatical v1.0\n");
    printf("Fichier source de Mini Project 'Compilateur'\n");
    printf("Licence d'utilisation  GPL : http://www.gnu.org/licenses/gpl.html \n");
    printf("\n\n\n");

    system("pause");
    
    char input[500];
    
    do{
        system("cls");
        printf("Enter une Affectation ou 'fin' pour Quiter\n");
        printf("BNF: <Nom-Variable> = <Expression> ;\n");
        printf("EXEMPLE: A = 22 + 6;\n");
        printf(">");
        gets(&input);
        if(strcmp(input,"fin") == 0)
            break;
        
        printf("Translation du expression en language 'Assembleur'\n");
        init(input);
        affectation();
        
    pass:
        system("pause");
    }while(VRAI);
    
    
    
    
    
    system("pause");
	return 0;
}
