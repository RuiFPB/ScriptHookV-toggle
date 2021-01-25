#include <stdio.h> 
#include <stdlib.h>
#include <string.h> // strcat() e strlen();
#include <unistd.h> // access() for the file check
#ifdef _WIN32
#include <windows.h> //SetConsoleOutputCP
#endif

#define STR_SIZE 300 // O tamanho é ligeiramente exagerado, mas não deve de ser problema tendo em conta que para jogar GTAV é necessário 8GB ou mais de memória.

#ifdef _WIN32
char base[STR_SIZE] = "move ";
#else
char base[STR_SIZE] = "mv ";
#endif
char installdir[STR_SIZE] = "";
char pathtofile[STR_SIZE] = "";
char npathtofile[STR_SIZE] = "";
char command[STR_SIZE] = "mv ";
char file2[12] = "dinput8.dll";
char file1[16] = "ScriptHookV.dll";
char disabler[9] = "_disable";
int opc = 0;
char inpt[STR_SIZE];
int end = { 0 };
int j = 0;

/*
Copia os conteúdos de s1 para s2
*/
void scopy(char s1[], char s2[]){
    for (int i = 0; i < STR_SIZE; i++) {
        s2[i] = s1[i];
    }
}

/*
Verifica se os ficheiros existem
*/
int status(char path[]){
    // printf("Now checking :%s \n", path);
    if (access (path, F_OK) == 0){
        return 1; // ScriptHook is enabled
    } else {
        strcat(path, disabler);
        // printf("Now checking :%s \n", path);
        if (access (path, F_OK) == 0){
            return 0; // ScriptHook is disabled
        } else return 2; // ScriptHook does not exist
    }
}

/*
Limpa a linha de commandos
*/
void clrscr(){
    #ifdef _WIN32
    system("cls");
    #else
    system("clear");
    #endif
}

/*
Aguarda que ENTER seja pressionado
*/
void waitenter(){
    while(getchar() != '\n');
}

/*
Ativa o ScriptHookV.dll e dinput8.dll
*/
void enable(char path[], int n){
    switch (n)
    {
    case 1:
        scopy(base, command);
        scopy(installdir, pathtofile);
        strcat(pathtofile, file1);
        scopy(pathtofile, npathtofile);
        strcat(npathtofile, disabler);
        strcat(command, npathtofile);
        command[strlen(command)] = ' ';
        strcat(command, pathtofile);
        printf("%s\n", command);
        system(command);
        printf("ScriptHookV was enabled!\n\n");
        break;
    case 2:
        scopy(base, command);
        scopy(installdir, pathtofile);
        strcat(pathtofile, file2);
        scopy(pathtofile, npathtofile);
        strcat(npathtofile, disabler);
        strcat(command, npathtofile);
        command[strlen(command)] = ' ';
        strcat(command, pathtofile);
        printf("%s\n", command);
        system(command);
        printf("dinput8 was enabled!\n");
        break;
    default:
        enable(path, 1);
        enable(path, 2);
        break;
    }
    ;
}

/*
Desativa o ScriptHookV.dll e dinput8.dll
*/
void disable(char path[], int n){
    switch (n)
    {
    case 1:
        scopy(base, command);
        scopy(installdir, pathtofile);
        strcat(pathtofile, file1);
        scopy(pathtofile, npathtofile);
        strcat(npathtofile, disabler);
        strcat(command, pathtofile);
        command[strlen(command)] = ' ';
        strcat(command, npathtofile);
        printf("%s\n", command);
        system(command);
        printf("ScriptHookV was disabled!\n\n");
        break;
    case 2:
        scopy(base, command);
        scopy(installdir, pathtofile);
        strcat(pathtofile, file2);
        scopy(pathtofile, npathtofile);
        strcat(npathtofile, disabler);
        strcat(command, pathtofile);
        command[strlen(command)] = ' ';
        strcat(command, npathtofile);
        printf("%s\n", command);
        system(command);
        printf("dinput8 was disabled!\n");
        break;
    default:
        disable(path, 1);
        disable(path, 2);
        break;
    }
    
}

// Função principal
int main(){
    #ifdef _WIN32
    SetConsoleOutputCP(65001); // Sets the codepage to utf_8 on windows 
    #endif
    printf("WARNING: This program should not be run when the game is running!\nThe program is delivered \"as is\" and any missuse is the user's fault!\n");
    waitenter();
    while (!end) {
        clrscr(); 
        printf("ScriptHookV toggler\n1. Check ScriptHook\n\n9. About\n0. Quit\n : ");
        fgets(inpt, STR_SIZE, stdin); opc = atoi(inpt);

        switch (opc)
        {
        case 1:
            if (installdir[0] == '\0') { // Se o diretório do jogo ainda não foi definido
                printf("Where is GTA V installed? : ");
                fgets(installdir, STR_SIZE, stdin);
                // remove \n do final do caminho
                if (installdir[strlen (installdir) - 1] == '\n')
                    installdir[strlen (installdir) - 1] = '\0';
                // se o caminho não teminar com \ ou / (dependendo do OS) acrescentar o carecter ao caminho
                #ifdef _WIN32
                if (installdir[strlen(installdir) - 1] != '\\')
                    installdir[strlen(installdir)] = '\\';
                #else
                if (installdir[strlen(installdir) - 1] != '/')
                    installdir[strlen(installdir)] = '/';
                #endif
            }

            // gerar o caminho para ScriptHookV.dll
            scopy(installdir, pathtofile);
            strcat(pathtofile, file1);

            // Verificar se o ficheiro existe. Se sim em que estado
            // printf("file to check: %s\n", pathtofile);
            j = status(pathtofile);
            switch (j){
            //ficheiro ativado
            case 1:
                scopy(installdir, pathtofile);
                strcat(pathtofile, file2);
                // printf("file to check2: %s\n", pathtofile);
                j = status(pathtofile);
                switch (j){
                case 1:
                    printf("ScriptHookV is enabled!\n");
                    printf("Toggle? [y/n]\n: ");
                    fgets(inpt, STR_SIZE, stdin);
                    if ((inpt[0] == 'y') || (inpt[0] == 'Y'))
                        disable(installdir, 3);
                    break;
                case 0: 
                    printf("ScriptHookV is enabled but dinput8.dll is disabled, both will be disabled!\n");
                    disable(installdir, 1);
                    break;
                }
                break;

            // ficheiro desativado
            case 0: 
                scopy(installdir, pathtofile);
                strcat(pathtofile, file2);
                // printf("file to check2: %s\n", pathtofile);
                j = status(pathtofile);
                switch (j){
                case 1:
                    printf("ScriptHookV is disabled but dinput8.dll is enabled, both will be disabled!\n");
                    disable(installdir, 2);
                    break;
                case 0: 
                    printf("ScriptHookV is disabled!\n");
                    printf("Toggle? [y/n]\n: ");
                    fgets(inpt, STR_SIZE, stdin);
                    if ((inpt[0] == 'y') || (inpt[0] == 'Y'))
                        enable(installdir, 3);
                    break;
                }
                break;
            case 2:
                printf("ScriptHook.dll is not in the path specified!\n");
                break;
            }
            break;
        // Acerca do programa
        case 9:
            printf("This program disables ScriptHookV.dll and the asi loader included with ScriptHook (dinput8.dll) to enable you to play GTA-Online.\n");
            printf("If you want to play GTAV with your scripts back, the same program is used to enable it back.\n");
            printf("No files get deleted, ScriptHookV.dll and dinput8.dll get _disabled added to the file extension, so they are not loaded by the game, and then get renamed back to their original state if you toggle it again.\n");
            break;
        // Sair
        case 0:
            end = 1;
            break;
        default:
            printf("That's not an option!\n");
            break;
        }

        if (opc != 0) waitenter();
    }
    printf("Goodbye!\n");    
}