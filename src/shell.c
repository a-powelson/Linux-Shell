#include "shell.h"

/* Fill history arrays cyclically, keep track of earliest (strt) and latest 
   (end) entries 
*/
void addToHistory(char cmdHist[HIST_LEN][MAX_LINE], char *cmd, 
                    int pidHist[HIST_LEN], int pid, int *end, int *strt) {
    // copy in most recent command, overwriting oldest if present
    strcpy(cmdHist[*end % HIST_LEN], cmd);

    // copy in most recent PID, overwriting oldest if present
    pidHist[*end % HIST_LEN] = pid;

    // increment end
    *end += 1;

    // only change strt once array is full
    if(*end > HIST_LEN)
        *strt = (*end % HIST_LEN);

    // keep end from growing indefinitely
    if(*end == 20)
        *end = 10;
}

/* Print command history beginning with most recent command 
*/
void printHistory(char cmdHist[HIST_LEN][MAX_LINE], int pidHist[HIST_LEN], 
                    int end, int strt) {
    int last = (end % HIST_LEN); /* oldest command index */
    int first = strt; /* most recent command index */
    int len; /* number of commands to print */

    // History is full
    if(end > 9)
        len = HIST_LEN - 1;
    // History is not full
    else
        len = abs(end - strt - 1);

    // Print history most to least recent
    int id = 1;
    printf("ID\tPID\tCommand\n");
    for(int i=(first + len); i>=first; i--) {
        printf("%d\t%d\t%s", id, pidHist[i % HIST_LEN], cmdHist[i % HIST_LEN]);
        id++;
    }

    fflush(stdout);
}

/* Parse agrument parameters from user input 
*/
void parseArgs(char* args[MAX_LINE/2 + 1], char* input) {
    args[0] = strtok(input, " \n");

    int i = 1;
    while(args[i-1] != NULL) {
        args[i] = strtok(NULL, " \n");
        i++;
    }

    args[i] = NULL;
}

/* Check user input against a given expression
    Return 0 if there's a match
    Return 1 if no match 
*/
int regex(char *input, char *exp) {
    regex_t regex;
    int regex_status;
    char msgbuf[MAX_LINE];

    /* Compile regular expression */
    regex_status = regcomp(&regex, exp, REG_EXTENDED);
    if (regex_status) {
        fprintf(stderr, "Could not compile regex\n");
        exit(1);
    }

    /* Execute regular expression */
    regex_status = regexec(&regex, input, 0, NULL, 0);
    if (!regex_status) {
        regfree(&regex);
        return 0;
    }
    else if (regex_status == REG_NOMATCH) {
        regfree(&regex);
        return 1;
    }
    else {
        regerror(regex_status, &regex, msgbuf, sizeof(msgbuf));
        fprintf(stderr, "Regex match failed: %s\n", msgbuf);
        regfree(&regex);
        return 1;
    }
}

/* (1) Attempt to create a child process and use it to 
       execute the given command
   (2) Add command & process ID to history
   (3) Parent waits for child 
*/
void makeChild(char* args[MAX_LINE/2 + 1], char *cmd, 
                char cmdHist[HIST_LEN][MAX_LINE], int pidHist[HIST_LEN], 
                int *end, int *strt, bool recall) {
    int wait_status; /* wait status of parent */
    int execvp_status; /* status of child's system call */
    pid_t pid = fork(); /* process id */

    // Failed fork
    if(pid < 0) {
        fprintf(stderr, "Fork Failed.");
        return;
    }

    // Child Process
    else if(pid == 0) {
        if(recall)
            printf("%s", cmd);

        // Parse argument
        parseArgs(args, cmd);

        // execute cmd
        execvp_status = execvp(args[0], args);

        // failed system call
        if(execvp_status == -1) {
            printf("Invalid command!\n");
            exit(1);
        }
    }

    // Parent process
    else {
        addToHistory(cmdHist, cmd, pidHist, pid, end, strt);
        wait(&wait_status);
    }
}
