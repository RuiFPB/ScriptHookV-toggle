// Programa que troca o estado 
// do ScriptHook.dll e do 
// dinput8.dll para que seja
// possível jogar online
// ou offline sem mods no GTA V
//
// Programado por: Rui Barbosa
// 31 Jan

#include <stdio.h> 
#include <stdlib.h>
#include <string.h> // strcat() e strlen();
#include <unistd.h> // access() for the file check

#include <windows.h> //SetConsoleOutputCP


#define STR_SIZE 256
    //Tamanho padrão das strings. O tamanho é ligeiramente exagerado mas assim asseguro que não exista erros mesmo quando o caminho para a instalação do jogo é mais longo.
	//Caso seja necessário mais caracteres para que o comando possa ser executado, apenas este valor terá que ser modificado.


char base[] = "move ";              // Comando base para mover ficheiros
char installdir[STR_SIZE] = "";     // Diretório de instalação do jogo
char pathtofile[STR_SIZE] = "";     // Caminho para o ficheiro
char npathtofile[STR_SIZE] = "";    // Novo caminho para o ficheiro
char command[STR_SIZE*2] = "";      // Comando completo
char file2[] = "dinput8.dll";       // Ficheiro a ser movido
char file1[] = "ScriptHookV.dll";   // Ficheiro a ser movido
char disabler[] = "_disable";       // Sufixo para desativar ficheiros
int opc = 0;                        // Opção escolhida
char inpt[STR_SIZE];                // Input do utilizador
int end = 0;                        // Flag para terminar o programa
int j = 0;                          // Variável temporária

/*
Copia os conteúdos de s1 para s2
mode 1 apenas copia até strlen(s1)
qualquer outro mode copia até STR_SIZE 
*/
void scopy(char s1[], char s2[], int mode){
    /* Funcionalidade mode foi acrescentada para corrigir um bug introduzido com a definição de installdir atravez de argv*/
    switch (mode)
    {
    case 1:
        for (int i = 0; i < strlen(s1); i++) {
            s2[i] = s1[i];
        }
        break;
    
    default:
        for (int i = 0; i < STR_SIZE; i++) {
            s2[i] = s1[i];
        }
        break;
    }
}

/*
Verifica se os ficheiros existem
*/
int status(char path[]){
    //printf("Now checking :%s \n", path);
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
TODO: Encontrar uma forma de limpar a consola sem usar system()
*/
void clrscr(){
    system("cls");
}

/*
Aguarda que ENTER seja pressionado
*/
void waitenter(){
    while(getchar() != '\n');
}

// Desativa ou ativa ficheiro
// 2 Ativa
// 1 Desativa
void toggle(char file[], int mode){
	scopy(base, command, 2);

	scopy(installdir, pathtofile, 2);
	strcat(pathtofile, file);

	scopy(pathtofile, npathtofile, 2);
	strcat(npathtofile, disabler);

	switch(mode){
		case 1:		// Desativar
			command[strlen(command)] = '"';     // Adiciona aspas ao início e fim do comando para evitar problemas com espaços no caminho
			strcat(command, pathtofile);
			command[strlen(command)] = '"';

			command[strlen(command)] = ' ';

			command[strlen(command)] = '"';
			strcat(command, npathtofile);
			command[strlen(command)] = '"';

			printf("> %s\n", command);
			system(command);
			printf("%s was disabled.\n", file);
			break;
		case 2:		// Ativar
			command[strlen(command)] = '"';
			strcat(command, npathtofile);
			command[strlen(command)] = '"';

			command[strlen(command)] = ' ';

			command[strlen(command)] = '"';
			strcat(command, pathtofile);
			command[strlen(command)] = '"';

			printf("> %s\n", command);
			system(command);
			printf("%s was enabled.\n", file);
			break;
		default:	// NADA, ABSOLUTAMENTE NADA
			break;
	}
}

// Função principal
int main(int argc, char **argv){
    SetConsoleOutputCP(65001); // Sets the codepage to utf_8 on windows 

    if (argc > 1) {
        scopy(argv[1], installdir, 1);
        printf("%s\n", installdir);
    }
    
    printf("WARNING: This program should not be run when the game is running!\nThe program is delivered \"as is\" and any missuse is the user's fault!\n");
    printf("Press ENTER to continue...");
    waitenter();
    while (!end) {
        clrscr(); 
        printf("ScriptHookV toggler\n\n1. Check ScriptHookV\n\n9. About\n0. Quit\n\n: ");
        fgets(inpt, STR_SIZE, stdin); opc = atoi(inpt);

        switch (opc){
        // Sair
        case 0:
            end = 1;
            break;
        
        // Verificação ScriptHook
        case 1:
            if (installdir[0] == '\0') { // Se o diretório do jogo ainda não foi definido
                printf("Where is GTA V installed? : ");
                fgets(installdir, STR_SIZE, stdin);
                // remove \n do final do caminho
                if (installdir[strlen (installdir) - 1] == '\n')
                    installdir[strlen (installdir) - 1] = '\0';
            }
            // se o caminho não teminar com \ ou / (dependendo do OS) acrescentar o carecter ao caminho
            if (installdir[strlen(installdir) - 1] != '\\')
                installdir[strlen(installdir)] = '\\';
            

            // gerar o caminho para ScriptHookV.dll
            scopy(installdir, pathtofile, 2);
            strcat(pathtofile, file1);

            // Verificar se o ficheiro existe. Se sim em que estado
            //printf("file to check: %s\n", pathtofile);
            j = status(pathtofile);
            switch (j){
            //ficheiro ativado
            case 1:
                scopy(installdir, pathtofile, 2);
                strcat(pathtofile, file2);
                // printf("file to check2: %s\n", pathtofile);
                j = status(pathtofile);
                switch (j){
                case 1:
                    printf("ScriptHookV is enabled!\n");
                    printf("Toggle? [y/n]\n: ");
                    fgets(inpt, STR_SIZE, stdin);
                    if ((inpt[0] == 'y') || (inpt[0] == 'Y')){
                        toggle(file1, 1);
		            	toggle(file2, 1);
		            }
                    else opc = 0;
                    break;
                case 0: 
                    printf("ScriptHookV is enabled but dinput8.dll is disabled, both will be disabled!\n");
                    toggle(file1, 1);
                    break;
	    	    case 2:
		            printf("dinput8.dll or dinput8.dll_disabled were not found. Check your ScriptHookV intalation!\n");
		            break;
                }
                break;

            // ficheiro desativado
            case 0: 
                scopy(installdir, pathtofile, 2);
                strcat(pathtofile, file2);
                // printf("file to check2: %s\n", pathtofile);
                j = status(pathtofile);
                switch (j){
                case 1:
                    printf("ScriptHookV is disabled but dinput8.dll is enabled, both will be disabled!\n");
		            toggle(file2, 1);
                    break;
                case 0: 
                    printf("ScriptHookV is disabled!\n");
                    printf("Toggle? [y/n]\n: ");
                    fgets(inpt, STR_SIZE, stdin);
                    if ((inpt[0] == 'y') || (inpt[0] == 'Y')){
			            toggle(file1, 2);
			            toggle(file2, 2);
		            }
                    else opc = 0;
                    break;
		        case 2:
		            printf("dinput8.dll or dinput8.dll_disabled were not found. Check your ScriptHookV instalation!\n");
		            break;
                }
                break;
            // Os ficheiros não foram encontrados
            case 2:
                printf("ScriptHookV.dll is not in the path specified!\n");
		        printf("Verify if the path is correct. If the path as spaces like \"Steam Library\" add quotes to begining and the end of the path (\".../.../GTAV\")\n");
                printf("If the path is correct verify if the files are on the correct place\n");
                break;
            }
            break;
        // Acerca do programa
        case 9:
            printf("This program disables ScriptHookV.dll and the asi loader included with ScriptHook (dinput8.dll) to enable you to play GTA-Online.\n");
            printf("If you want to play GTAV with your scripts back, the same program is used to enable it back.\n");
            printf("No files get deleted, ScriptHookV.dll and dinput8.dll get _disabled added to the file extension, so they are not loaded by the game, and then get renamed back to their original state if you toggle it again.\n");
            break;
        default:
            printf("That's not an option!\n");
            break;
        }

        if (opc != 0) waitenter();
    }
    printf("Goodbye!\n");
}
