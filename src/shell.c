#include <stdio.h> 
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

// buffer size for user input
int buffer = 4096;
// signal flag for Ctrl+C interruption
int ctrlc = 0; 

// change the current working directory
void change_directory(char* path) {
    if(chdir(path) != 0) perror("Error"); 
}

void waiting(char* processId[]) {
    /**
     * Wait for specific processes to finish
    */
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

void execute_pipe(char* prog1[], char* prog2[]) {
    /**
     * Execute processes connected by a pipe
    */
    int pipefd[2];
    pid_t pid1, pid2;

    // create the pipe
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(-1);
    }

    // fork the first process
    pid1 = fork();
    if (pid1 == -1) {
        perror("fork");
        exit(-1);
    } else if (pid1 == 0) {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        execvp(prog1[0], prog1);
        perror("execvp");
        exit(EXIT_FAILURE);
    }

    // fork the second process
    pid2 = fork();
    if (pid2 == -1) {
        perror("fork");
        exit(-1);
    } else if (pid2 == 0) {
        close(pipefd[1]);
        dup2(pipefd[0], STDIN_FILENO);
        execvp(prog2[0], prog2);
        perror("execvp");
        exit(-1);
    }

    // parent process
    close(pipefd[0]);
    close(pipefd[1]);

    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
}

int run(char* input[], int execInBackground1) {
    /**
     * Run a single process or pipe two, with or without background execution
    */
    int status;
    pid_t pid = fork();

    if(pid < 0) {
        perror("Error");
        exit(-1);
    }
    
    if(pid == 0) {
        int i = 0;
        int pipeIndex = -1;
        while (input[i] != NULL) {
            if (strcmp(input[i], "|") == 0) {
                pipeIndex = i;
                break;
            }
            i++;
        }

        if (pipeIndex != -1) {
            input[pipeIndex] = NULL;
            char** prog1 = input;                   
            char** prog2 = &input[pipeIndex + 1];
            execute_pipe(prog1, prog2);
            exit(0);
        } else {
            execvp(input[0], input);
            exit(-1);
        }
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
    // input with buffer
    char input[buffer];
    // args to hold arguments, args[0] = program name 
    char* args[buffer]; 

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

        // remove Enter from input
        input[strcspn(input, "\n")] = '\0'; 

        char* token;
        token = strtok(input, " ");
        int i = 0;
        while(token != NULL) {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL; 

        if(strcmp(input, "exit") == 0) {
            break;
        }
        if(strcmp(input, "cd") == 0) {
            change_directory(args[1]);
            continue;
        }
        // check if process should be executed in background
        if(strcmp(args[i - 1], "&") == 0) {  
            args[i - 1] = "\0"; // replace "&" 
            run(args, 1); // "&" --> run in background
            execInBackground = 0; // reset
            continue;
        }
        if(strcmp(input, "wait") == 0) {
            // pass the whole line including "wait", iterate from index 1 in waiting()
            waiting(args); 
            continue;
        }
        
        execInBackground = 0;
        run(args, execInBackground);
    }
    return 0;   
}
