#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>
//#include <ncurses.h>    // must have ncurses downloaded (no alpine distro); call w/ "g++ cwushell.c -o cwushell -lncurses"

struct StackNode {
    char command[100];
    struct StackNode* next;
};
 
struct StackNode* newNode(char command[]) {
    struct StackNode* stackNode = 
      (struct StackNode*)
      malloc(sizeof(struct StackNode));
    strcpy(stackNode->command,command);
    stackNode->next = NULL;
    return stackNode;
}
 
int isEmpty(struct StackNode* root) {
    return !root;
}
 
void push(struct StackNode** root, char command[]) {
    struct StackNode* stackNode = newNode(command);
    stackNode->next = *root;
    *root = stackNode;
}
 
char* peek(struct StackNode* root) {
    if (isEmpty(root))
        return NULL;
    return root->command;
}

char* pop(struct StackNode** root) {
    if (isEmpty(*root))
        return NULL;
    struct StackNode* temp = *root;
    *root = (*root)->next;
    char* popped; 
    strcpy(popped,temp->command);
    free(temp);
    return popped;
}

// Invoke to initialize the shell.
int init() {
    int n;  // max amount of commands stored in history
    printf("\033[H\033[J"); // clears the console
    printf("Enter a value for n: ");
    fflush(stdout);
    char temp = getchar();
    if(isdigit(temp) == 1) {
        n = temp - '0';
    } else {
        n = 5;
    }
    printf("\033[H\033[J"); // clears the console
    printf("Project: CS 470 - Lab 1\n");
    printf("Author: Zachary Dehaan\n");
    printf("\n\nLaunching cwushell...\n\n");
    printf("\nFor a list of commands, type \"manual\".\n\n");
    return n;
}

// Invoke to display the shell manual.
void display_manual() {
    printf("\n====== CWU SHELL MANUAL ======\n\n");
    printf("Commands:\n\n");
    printf("\tsysteminformation -switch\n");
    printf("\t   Switches: -p, -m, -o\n\n");
    printf("\tmemoryinformation -switch\n");
    printf("\t   Switches: -s, -u, -S\n\n");
    printf("\texit [n]\n\n");
    printf("\thistory [n]\n\n");
    printf("\t<command> <parameters>\n");
    printf("\t   Executes the command prompt with the given parameters.\n\n");
    printf("To get information on a specific command, type -h or --help as a switch\n");
}

// Invoke to display the history of commands.
void display_history(int n, struct StackNode** topper) {
    printf("Displaying the last %d commands. If there are less than %d, command history will print all. 1 is the latest executed command.\n\n", n, n);
    struct StackNode* curr_node = *topper; // top of stack
    int count = 0;
    while(count < n ) {
        if(curr_node->command != NULL) {
            printf("%d. %s\n",(count+1),curr_node->command);
            if(curr_node->next != NULL) {
                *curr_node = *(curr_node->next);
            } else {
                return;
            }
            count++;
        } else {
            return;
        }
    }
}

// Invoke to display help for the command systeminformation.
void display_help_si() {
    printf("\n\"systeminformation\" has the following switches:\n");
    printf("\t[-p] display number of processors available in the system\n");
    printf("\t[-m] display maximum number of open files per process\n");
    printf("\t[-o] display the processor type\n");
}

// Invoke to display help for the command memoryinformation.
void display_help_mi() {
    printf("\n\"memoryinformation\" has the following switches:\n");
    printf("\t[-s] display total swap space in bytes\n");
    printf("\t[-u] display total memory in bytes\n");
    printf("\t[-S] display total shared memory in bytes available in the system\n");
}

// Invoke to display help for the command exit [n].
void display_help_exit() {
    printf("\n\"exit\" can have the parameter n:\n");
    printf("\t[n] return the exit value of shell's execution\n");
    printf("\tOtherwise, return the exit value of the last executed command (or 0 if no previous commands)\n");
}

// Invoke to run the command "systeminformation -p".
int sysinf_p() {
    int code = system("nproc --all");
    return code;
}

// Invoke to run the command "systeminformation -m".
int sysinf_m() {
    int code = system("ulimit -n");
    return code;
}

// Invoke to run the command "systeminformation -o".
int sysinf_o() {
    int code = system("cat /proc/cpuinfo | grep 'model name' | uniq");
    return code;
}

// Invoke to run the command "memoryinformation -s".
int meminf_s() {
    int code = system("free -b | egrep 'total|Swap'");
    return code;
}

// Invoke to run the command "memoryinformation -u".
int meminf_u() {
    int code = system("free -b | egrep 'total|Mem'");
    return code;
}

// Invoke to run the command "memoryinformation -S".
int meminf_capS() {
    int code = system("free -t -b | egrep 'total|Total'");
    return code;
}

// Starts the shell and executes in a loop until the exit command is reached.
int shell_loop(int n) {
    // initialize stack
    struct StackNode* top = NULL;
    char full_command[95];
    int exit_code = 0;
    int status = 1;
    //initscr();
    //noecho();
    //keypad(stdscr, TRUE);
    //cbreak();
    while(status) {
        printf("\ncwushell> ");
        fflush(stdout);

        /**
        int c = getch();
        if(c == KEY_UP || c == KEY_DOWN) {
            struct StackNode* temp = top;
            char prev[95];
            int cnt = 0;
            while(c == KEY_UP || c == KEY_DOWN && cnt < n) {
                printw("%c[2K", 27);    // clear current line
                if(c == KEY_UP) {
                    if(temp->command != NULL) {
                        strcpy(command,temp->command);
                        *temp = *(temp->next);
                    }
                } else if(c == KEY_DOWN) {
                    
                }
                printw("cwushell> %s",command);
                cnt++;
                c = getch();
            }
            fflush(stdout);
        } else {
            printw(" %c",(char)c);
            fflush(stdout);
            fgets(command, sizeof(command), stdin);
        }
        char full_command[105];
        if(isascii(c) == 1) {
            char start = c;
            // add command to history using a Stack
            sprintf(full_command,"%c%s",(char)start,command);
        } else {
            strcpy(full_command,command);
        }**/
        //char full_command[105];
        
        /**int c = getchar();
        printf("char: %c", c);
        if(c == '0' || c == '1') {
            struct StackNode* temp = top;
            int cnt = 0;
            while(c == '0' && cnt < n) {
                printf("%c[2K", 27);    // clear current line
                if(temp->command != NULL) {
                    strcpy(command,temp->command);
                    *temp = *(temp->next);
                }
                printf("cwushell> %s",command);
                cnt++;
                c = getchar();
            }
        } else {
            if(isascii(c) == 1) {
                sprintf(full_command,"%c%s",(char)c,command);
            } else {
                strcpy(full_command,command);
            }
        }**/

        fgets(full_command, sizeof(full_command), stdin);
        //strcpy(full_command,command);
        push(&top,full_command);
        char* token;

        // ######## system-information ########
        if(strstr(full_command,"systeminformation") != NULL) {
            // system-information -p
            if(strcmp(full_command,"systeminformation -p\n") == 0 || strstr(full_command,"-p\n") != NULL) {
                printf("Total number of processors available in the system: ");
                fflush(stdout);
                exit_code = sysinf_p();
            }
            // system-information -m
            else if(strcmp(full_command,"systeminformation -m\n") == 0 || strstr(full_command,"-m\n") != NULL) {
                printf("Maximum number of open files per process: ");
                fflush(stdout);
                exit_code = sysinf_m();
            }
            // system-information -o
            else if(strcmp(full_command,"systeminformation -o\n") == 0 || strstr(full_command,"-o\n") != NULL) {
                printf("Processor type: ");
                fflush(stdout);
                exit_code = sysinf_o();
            }
            // system-information --help / -h
            else if(strcmp(full_command,"systeminformation -h\n") == 0 || strstr(full_command,"systeminformation --help\n") != NULL || strstr(full_command,"-h\n") != NULL || strstr(full_command,"--help\n") != NULL) {
                display_help_si();
            }
            // erroneous switch!!
            else {
                printf("Your command: %s had a unexpected switch. Please try again.\n", full_command);
                exit_code = -1;
            }
        }

        // ######## memoryinformation ########
        else if(strstr(full_command,"memoryinformation") != NULL) {
            // memoryinformation -s
            if(strcmp(full_command,"memoryinformation -s\n") == 0 || strstr(full_command,"-p\n") != NULL) {
                printf("Total swap space in bytes:\n");
                fflush(stdout);
                exit_code = meminf_s();
            }
            // memoryinformation -u
            else if(strcmp(full_command,"memoryinformation -u\n") == 0 || strstr(full_command,"-u\n") != NULL) {
                printf("Total memory in bytes:\n");
                fflush(stdout);
                exit_code = meminf_u();
            }
            // memoryinformation -S
            else if(strcmp(full_command,"memoryinformation -S\n") == 0 || strstr(full_command,"-S\n") != NULL) {
                printf("Total shared memory in bytes available in the system:\n");
                fflush(stdout);
                exit_code = meminf_capS();
            }
            // memoryinformation -help / -h
            else if(strcmp(full_command,"memoryinformation -h\n") == 0 || strcmp(full_command,"memoryinformation -help\n") == 0 || strstr(full_command,"-h\n") != NULL || strstr(full_command,"-help\n") != NULL) {
                display_help_mi();
            }
            // erroneous switch!!
            else {
                printf("Your command: %s had a unexpected switch. Please try again.\n", full_command);
                exit_code = -1;
            }
        }

        // ######## exit prompt ########
        else if(strstr(full_command,"exit") != NULL) {
            // exit -help / -h
            if(strcmp(full_command,"exit -h\n") == 0 || strcmp(full_command,"exit -help\n") == 0 || strstr(full_command,"-h\n") != NULL || strstr(full_command,"-help\n") != NULL) {
                display_help_exit();
            }
            // exit [n]
            else if(strcmp(full_command,"exit\n") != 0){
                token = strtok(full_command, " ");
                while(token != NULL) {
                    token = strtok(NULL, " ");
                    if(isdigit(*token) == 1) {
                        exit_code = atoi(token);
                        //endwin();
                        return exit_code;
                    }
                    token = strtok(NULL, " ");
                }
                printf("Your command: %s had a unexpected switch. Please try again.\n", full_command);
                exit_code = -1;
            }
            // exit
            else if(strcmp(full_command,"exit\n") == 0) {
                //endwin();
                return exit_code;
            }
        }

        // ######## manual ########
        else if(strcmp(full_command,"manual\n") == 0 || strstr(full_command,"manual") != NULL) {
            display_manual();
        }

        // ######## clear ########
        else if(strcmp(full_command,"clear\n") == 0 || strstr(full_command,"clear") != NULL) {
            printf("\033[H\033[J"); // clears the console
        }

        // ######## history ########
        else if(strstr(full_command,"history") != NULL) {
            // history
            if(strcmp(full_command,"history\n") == 0) {
                display_history(n,&top);
            }

            // history [n]
            else {
                token = strtok(full_command, " ");
                token = strtok(NULL, " ");
                if(isdigit(*token)) {
                    if(atoi(token) < n) {
                        display_history(atoi(token),&top);
                    } else {
                        display_history(n,&top);
                    }
                } else {
                    printf("Your command: %s had a unexpected switch. Please try again.\n", full_command);
                    exit_code = -1;
                }
            }
        }

        // ######## other existing commands ########
        else {
            exit_code = system(full_command);
        }
        //endwin();
    }
    return 0;
}

int main() {
    int n = init();
    int retVal = shell_loop(n);
    return retVal;
}
