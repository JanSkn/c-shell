#include <stdio.h> 
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

int buffer = 100;
int ctrlc = 0; 

void change_directory(char* path) {
    if(chdir(path) != 0) perror("Error"); 
}

void waiting(char* processId[]) {
    if(!ctrlc) {
        int status;
        int i = 1;
        int proc;
        while (processId[i] != NULL) {
            proc = atoi(processId[i]);
            printf("Waiting for process %d\n", proc);
            pid_t pid = waitpid(proc, &status, 0);
            if (pid > 0) {             
                if (WIFEXITED(status)) {
                    printf("[%d] TERMINATED\n", pid);
                    printf("[%d] EXIT STATUS: %d\n", pid, WEXITSTATUS(status));
                } 
            }
            
            if (pid < 0) {
			    perror("Error");
			    exit(-1);
		    }
            i++;
        }
    }   
    ctrlc = 0;
}

int run(char* input[], int execInBackground1) {
    int status;
    pid_t pid;

    if(pid < 0) {
        perror("Error");
        exit(-1);
    }
    
    if((pid = fork()) == 0) {
        execvp(input[0], input); 
        exit(-1);
    }
    
    // parent
    if(execInBackground1 != 1) {
        waitpid(pid, &status, 0);
    } else {
        printf("[%u]\n", pid);
    }

    return WEXITSTATUS(status);
}

void sig_handler(int signo) {
    ctrlc = 1;
}

int main() {
    char input[buffer]; // Input mit Puffer
    char* args[buffer]; // enthält pro Index ein arg, args[0] = Programmname

    signal(SIGINT, sig_handler);
    int execInBackground = 0;
    #define ANSI_COLOR_NEONGREEN "\x1b[38;2;0;255;0m"   
    #define ANSI_COLOR_RESET "\x1b[0m"  

    while(1) {
        char* directory = getcwd(NULL, 0);
		printf(ANSI_COLOR_NEONGREEN"%s/>"ANSI_COLOR_RESET, directory);

        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("\n");
            break; 
        }

        input[strcspn(input, "\n")] = '\0'; // Enter aus Input entfernen

        char* token;
        token = strtok(input, " ");
        int i = 0;
        while(token != NULL) {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL; // kennzeichnet Ende der Liste von Token

        if(strcmp(input, "exit") == 0) {
            break;
        }
        if(strcmp(input, "cd") == 0) {
            change_directory(args[1]);
            continue;
        }
        if(strcmp(args[i - 1], "&") == 0) {  // *args[i-1] == '&' äquivalent
            args[i - 1] = "\0"; // "&" ersetzEn
            run(args, 1); // "&" --> im Hintergrund ausführen
            execInBackground = 0; // wieder zurücksetzen für nächsten Durchlauf
            continue;
        }
        if(strcmp(input, "wait") == 0) {
            waiting(args); // ganze Zeile inkl. "wait" wird übergeben, daher in waiting(): ab Index 1 iterieren
            continue;
        }
        
        execInBackground = 0;
        run(args, execInBackground);
    
        // TODO: Piping
    }
    return 0;   
}