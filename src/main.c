#include "shell.h"

int main(void) {
    char *args[MAX_LINE/2 + 1]; /* an array of character strings */
    char input[MAX_LINE]; /* user input */
    int should_run = 1; /* flag to determine when to exit program */
    char cmdHistory[HIST_LEN][MAX_LINE]; /* history array for commands */
    int pidHistory[HIST_LEN]; /* history array for PID */
    int end = 0, strt = 0; /* start/end pointers for history arrays */

    while (should_run) {
        // Get user input
        printf("CSCI3120> ");
        fflush(stdout);
        fgets(input, MAX_LINE, stdin);

        // Exit
        if(strcmp(input, "exit\n") == 0)
            should_run = 0;

        // Display history
        else if(strcmp(input, "history\n") == 0) {
            if(end > 0)
                printHistory(cmdHistory, pidHistory, end, strt);   

            else
                printf("No command in history!\n");
        }

        // Recall most recent (!!) or Nth most recent (!N) command
        else if(regex(input, "!(!|[[:digit:]])") == 0) {
            // Most recent
            if(strcmp(input, "!!\n") == 0) {
                if(end == 0)
                    printf("No command in history!\n");
                    
                else {
                    char * cmd = cmdHistory[end % HIST_LEN - 1];
                    makeChild(args, cmd, cmdHistory, pidHistory, &end, &strt, IS_RECALL);
                }
            }

            // Nth most recent
            else {
                int histID = atoi(strtok(input, "!!"));

                // history is empty
                if(end == 0 && strt ==0)
                    printf("No command in history!\n");

                // invalid ID
                else if(histID < 1 || histID > 10)
                    printf("Such a command is NOT in history!\n");

                // not enough history yet
                else if(end < 10 && histID > end)
                    printf("Such a command is NOT in history!\n");

                // execute Nth command
                else {
                    char * cmd = cmdHistory[(end - histID) % HIST_LEN];
                    makeChild(args, cmd, cmdHistory, pidHistory, &end, &strt, IS_RECALL);
                }
            }
        }

        // Execute given command
        else
            makeChild(args, input, cmdHistory, pidHistory, &end, &strt, !IS_RECALL);
    }

    return 0;
}
