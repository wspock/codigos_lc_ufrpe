#include <stdio.h>
#include <locale.h>
#include <string.h>

int main(){
    setlocale(LC_ALL, "portuguese");

    #ifdef _WIN32
        #define LIMPA_TELA system("cls")
    #else
        #define LIMPA_TELA system("clear")
    #endif

    char verbo[20];
    char resposta;

    do{
        LIMPA_TELA;

        printf("----------------------------------------------------------------------------------------------------------------------\n");
        printf("                                            Conjugador de Verbos 1.0\n");
        printf("----------------------------------------------------------------------------------------------------------------------\n\n");

        printf(" Digite um verbo no infinitivo (somente verbos regulares): ");
        scanf("%s", verbo);

        infinitivoParaConjugado(verbo);

        printf("\n\n Deseja digitar outro verbo? (S/N): ");
        scanf(" %c", &resposta); // Colocar espa�o antes de %c faz com que reconhe�a o caractere corretamente;
    }while(resposta == 'S' || resposta == 's');

    printf("\n\n Obrigado. At� logo!\n\n");

    return 0;
}

void infinitivoParaConjugado(char verbo[]){
    char radical[20];
    char sufixo[3];

    strncpy(radical, verbo, strlen(verbo) - 2);
    radical[strlen(verbo) - 2] = '\0';

    strcpy(sufixo, verbo + strlen(verbo) - 2);

    printf("\n\n"); // Quebra de linha para ficar mais vis�vel;

    // Eu;
    if(strcmp(sufixo, "ar") == 0){
        printf(" Eu %so\n", radical);
    }else if (strcmp(sufixo, "er") == 0){
        printf(" Eu %so\n", radical);
    }else if (strcmp(sufixo, "ir") == 0){
        printf(" Eu %so\n", radical);
    }

    // Tu;
    if(strcmp(sufixo, "ar") == 0){
        printf(" Tu %sas\n", radical);
    }else if (strcmp(sufixo, "er") == 0){
        printf(" Tu %ses\n", radical);
    }else if (strcmp(sufixo, "ir") == 0){
        printf(" Tu %ses\n", radical);
    }

    // Ele;
    if(sufixo[0] == 'a'){
        printf(" Ele %sa\n", radical);
    }else if (sufixo[0] == 'e'){
        printf(" Ele %se\n", radical);
    }else if (sufixo[0] == 'i'){
        printf(" Ele %se\n", radical);
    }

    // N�s;
    if(strcmp(sufixo, "ar") == 0){
        printf(" N�s %samos\n", radical);
    }else if (strcmp(sufixo, "er") == 0){
        printf(" N�s %semos\n", radical);
    }else if (strcmp(sufixo, "ir") == 0){
        printf(" N�s %simos\n", radical);
    }

    // V�s;
    if(strcmp(sufixo, "ar") == 0){
        printf(" V�s %sais\n", radical);
    }else if (strcmp(sufixo, "er") == 0){
        printf(" V�s %seis\n", radical);
    }else if (strcmp(sufixo, "ir") == 0){
        printf(" V�s %sis\n", radical);
    }

    // Eles;
    if(strcmp(sufixo, "ar") == 0){
        printf(" Eles %sam\n", radical);
    }else if (strcmp(sufixo, "er") == 0){
        printf(" Eles %sem\n", radical);
    }else if (strcmp(sufixo, "ir") == 0){
        printf(" Eles %sem\n", radical);
    }
}
