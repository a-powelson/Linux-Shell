#ifndef SHELL_HEADER
#define SHELL_HEADER

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <regex.h>
#include <stdbool.h>
#define MAX_LINE 80 /* The maximum length command */
#define HIST_LEN 10 /* maxmimum length of cmd history */
#define IS_RECALL true /* whether or not user has input !! or !N */

void addToHistory(char[HIST_LEN][MAX_LINE], char*, int*, int, int*, int*);
void printHistory(char[HIST_LEN][MAX_LINE], int[HIST_LEN], int, int);
void parseArgs(char*[MAX_LINE/2 + 1], char*);
int regex(char*, char*);
void makeChild(char*[MAX_LINE/2 + 1], char*, char[HIST_LEN][MAX_LINE], int[HIST_LEN], int*, int*, bool);

#endif
