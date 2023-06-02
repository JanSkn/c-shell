#include <stdio.h> 
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

#include "array.h"

int buffer = 100;

void change_directory(char* path) {
    if(chdir(path) != 0) perror("Error changing directory"); // gibt 0 bei Erfolg zurück
}

void waiting(pid_t* pidArray, int pidArrayLength) {
    int status;
    pid_t pid;
    
    for(int i = 0; i < pidArrayLength; i++) {
        waitpid(pidArray[i], &status, 0);
        if(WIFEXITED(status)) printf("Process %d exited.\n", pidArray[i]);
    }
}

int run(char* input, int execInBackground) {
    // To Do: mit strtok nach | slicen, einzelne teile in array speichern, pipen, execvp - problem: programmparameter? - ganzer Pfad nicht nötig, da cd
    char* inputToken = strtok(input, " | "); // ganzer String, wenn keine Pipe enthalten, Trennzeichen inkl. Leerzeichen, damit nicht im Befehl
    
    if(input[0] == '.' && input[1] == '/') {
        inputToken = input + strlen("./");
        //printf("prog: %s\n", inputToken);
        //execvp
        // return
        // denn dann nicht weiter zur pipe da entweder | oder ./
    }
    
    char* inputArray[buffer];
    int iterator = 0;
    while(inputToken != NULL) { // gibt Pointer auf erstes Token im String zurück --> inputToken = strtok(NULL, " "); aktualisieren
        inputArray[iterator++] = inputToken;
        inputToken = strtok(NULL, " | "); 
    }

    int pipe_fd[2];
    for(int i = 0; i < iterator; i++) {
        pipe(pipe_fd);
        pid_t pid = fork();
        if(pid < 0) {
            fprintf(stderr, "Error forking\n");
            exit(-2);
        }
        if(pid == 0) {
            dup2(pipe_fd[1], STDOUT_FILENO);
            close(pipe_fd[0]); // wird nicht benutzt
            close(pipe_fd[1]); // schließen, da jetzt alles auch bei STDOUT_FILENO
            char* args[buffer];
            int programIterator = 0;
            char* programToken = strtok(inputArray[i], " "); // jeweils erstes Token zwischen Pipe: Programmname
            while(programToken != NULL) { // gibt Pointer auf erstes Token im String zurück --> inputToken = strtok(NULL, " "); aktualisieren
                args[programIterator++] = inputToken; // --> die restlichen Tokens zwischen Pipe sind die args
                programToken = strtok(NULL, " "); 
            }
            execvp(args[0], args); // problem: args ohne args[0] sind die args
            perror("Error executing program:\n");
            exit(-1);
        }
        // hier parent:
        if(execInBackground == 0) {
            waitpid(pid, NULL, 0);
        }
    }
    //printf("%s\n",inputArray[0]);
}

void sig_handler(int signo) {
    const char msg[] = "CTRL + C - stop waiting\n";
    write(2, msg, sizeof(msg) - 1); // globale Variable hinzufügen, die auf 0 gesetzt wird undin funktion waiting nur ausgeführt solange !=0?

}

int main() {
    char input[buffer]; // Input mit Puffer
    char input_copy[buffer]; // Kopie, da strtok String verändert

    signal(SIGINT, sig_handler);

    while(1) {
        int execInBackground = 0;

        char* directory = getcwd(NULL, 0);
		printf("%s>", directory);

        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("\n");
            break;
        }
        
        strcpy(input_copy, input);
        char* firstWord = strtok(input_copy, " ");

        input[strcspn(input, "\n")] = '\0'; // Enter aus Input entfernen

        if(strcmp(input, "exit") == 0) break;
        
        if(strcmp(firstWord, "cd") == 0) {
            directory = input + strlen("cd") + 1; // Pointer zeigt auf Eingabe nach cd, + 1, damit kein Leerzeichen vor Pfad
            change_directory(directory);
        }

        if(input[strlen(input) - 1] == '&') {
            input[strlen(input) - 1] = '\0'; 
            execInBackground = 1;
        }

        //if(input[0] == '.' && input[1] == '/') {
        //    directory = input + strlen("./"); // Pointer zeigt auf Eingabe nach ./
        //}

        if(strcmp(firstWord, "wait") == 0) {
            char* pids = input + strlen("wait") + 1; // Pointer zeigt auf Eingabe nach wait, + 1, damit kein Leerzeichen vor erstem Wert
            char* pidsToken = strtok(pids, " ");
            pid_t pidArray[buffer];
            int iterator = 0;
            while(pidsToken != NULL) { // gibt Pointer auf erstes Token im String zurück --> pidsToken = strtok(NULL, " "); aktualisieren
                pidArray[iterator++] = (int)pidsToken;
                pidsToken = strtok(NULL, " "); 
            }
            waiting(pidArray, iterator);
        }   

        run(input, execInBackground); // wenn alle anderen eingaben nicht: dann programm ausführen --> noch ./ bereinigen, falls keine pipe
    }
}