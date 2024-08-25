#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define max_cmd_length 100

int main() {
    char input[max_cmd_length];

    while (1) {
        printf("osh> "); // Print prompt
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        // Remove the newline character at the end of the input
        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "exit") == 0) {// Exit the shell if the user enters "exit"
            break;
        }
        else {
            pid_t pid = fork();
            if (pid == 0) {// Child process
                char *args[64]; // Maximum number of arguments
                int count = 0;
                char *token = strtok(input, " ");
                while (token != NULL) {
                    args[count++] = token;
                    token = strtok(NULL, " ");
                }
                args[count] = NULL;
                execvp(args[0], args); // Execute the command in the child process

                perror("exec error"); // If execvp() fails, print an error message
                exit(EXIT_FAILURE);   // Terminate the child process
            }
            else if (pid == -1) {
                perror("fork error");
                exit(EXIT_FAILURE);
            }
            else {// Parent process
                wait(NULL);
            }
        }
    }

    return 0;
}
