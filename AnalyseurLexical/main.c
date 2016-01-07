#include <stdio.h>
#include <windows.h>
#include "analyseurLexical.h"


#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

char input[1024];

void lire_clavier(){
    printf("Entrer une phrase : \n");
    gets(input);
    system("cls");
}

void debut(){
    
    lire_clavier(); // remplire source
    init(input);
}

int main(int argc, char **argv)
{
    
    printf("Analyseur Lexical v1.0\n");
    printf("Fichier source de Mini Project 'Compilateur'\n");
    printf("Licence d'utilisation  GPL : http://www.gnu.org/licenses/gpl.html \n");
    printf("\n\n\n");
    debut();
	LEXLEME L;
    
    
    printf("### %s \n\n",input);
    do{
        L = obtenir_lexeme();
        if(L.type == LEXEME_TYPE.FIN)
            break;
        //printf(ANSI_COLOR_RED "%s" ANSI_COLOR_RESET "- type: %d - de: %d a: %d \n", L.valeur, L.type, L.debut, L.fin);
        printf(ANSI_COLOR_RED "%s" ANSI_COLOR_RESET, L.valeur);
        
        printf(" - type: ");
        switch(L.type){
            case 1:
                printf( ANSI_COLOR_YELLOW "Identificateur" ANSI_COLOR_RESET );
                break;
            case 3:
                printf( ANSI_COLOR_YELLOW "Nombre Entier" ANSI_COLOR_RESET );
                break;
            case 4:
                printf( ANSI_COLOR_YELLOW "Nombre Decimale" ANSI_COLOR_RESET );
                break;   
            case 51:
            case 52:
            case 53:
            case 54:
            case 55:
            case 56:
            case 57:
            case 58:
            case 59:
            case 60:
            case 61:
            case 89:
                printf( ANSI_COLOR_YELLOW "Mot Cle" ANSI_COLOR_RESET );
                break;
            
            case 70:
            case 71:
            case 72:
                printf( ANSI_COLOR_YELLOW "Type de Variable" ANSI_COLOR_RESET );
                break;
                
            case 90:
            case 91:
                printf( ANSI_COLOR_YELLOW "Fonction Predifinie" ANSI_COLOR_RESET );
                break;
                
            case 98:
                printf( ANSI_COLOR_YELLOW "Fin du Line" ANSI_COLOR_RESET );
                break;

            case 99:
                printf( ANSI_COLOR_YELLOW "Fin du fichier" ANSI_COLOR_RESET );
                break;
                
            default:
                printf( ANSI_COLOR_YELLOW "Operateur" ANSI_COLOR_RESET,L.type );
        }
        printf("- de: " ANSI_COLOR_MAGENTA "%d" ANSI_COLOR_RESET,L.debut);
        printf(" a: " ANSI_COLOR_MAGENTA "%d" ANSI_COLOR_RESET,L.fin);
        
         printf("\n");
        
    }while(1);
    
    
	system("pause");
    return 0;
}
