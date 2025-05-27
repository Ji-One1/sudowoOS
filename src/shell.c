#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shell.h"
#include "interpreter.h"
#include "shellmemory.h"

int parseInput(char ui[]);

// Start of everything
int main(int argc, char *argv[]) {
    printf("Shell version 1.3 Created September 2024\n");
    help();

    char prompt = '$';  				// Shell prompt
    char userInput[MAX_USER_INPUT];		// user's input stored here
    int errorCode = 0;					// zero means no error, default
    int interactive = isatty(fileno(stdin)); // 0 for batch, 1 for interactive
    FILE *input = stdin; // current input source

    //init user input
    for (int i = 0; i < MAX_USER_INPUT; i++) {
        userInput[i] = '\0';
    }

    //init shell memory
    mem_init();
    while(1) {
        if (interactive) printf("%c ", prompt);
        //check for EOF for batch mode
        if (fgets(userInput, MAX_USER_INPUT-1, input) == NULL && !interactive) { 
            input = fopen("/dev/tty", "r");
            interactive = 1;
            continue;
        };

        errorCode = parseInput(userInput);
        if (errorCode == -1) exit(99);	// ignore all other errors
        memset(userInput, 0, sizeof(userInput));
    }

    return 0;
}

int wordEnding(char c) {
    // You may want to add ';' to this at some point,
    // or you may want to find a different way to implement chains.
    return c == '\0' || c == '\n' || c == ' ';
}

int parseInput(char inp[]) {
    char tmp[200], *words[100];
    int ix = 0, w = 0, tWordLen = 0;
    int wordlen;
    int errorCode;
    char *command = strtok(inp, ";");
    
    while (command != NULL) {
        for (ix = 0; command[ix] == ' ' && ix < 1000; ix++); // skip white spaces
        while (command[ix] != '\n' && command[ix] != '\0' && tWordLen < 1000) {

            // extract a word
            for (wordlen = 0; !wordEnding(command[ix]) && tWordLen < 1000; ix++, wordlen++) {
                tmp[wordlen] = command[ix];
                tWordLen += wordlen;
            }
            tmp[wordlen] = '\0';
            words[w] = strdup(tmp);
            w++;

            if (command[ix] == '\0') break;
            ix++;
        }
        errorCode = interpreter(words, w);
        command = strtok(NULL, ";");
        w = 0;
    }
    return errorCode;
}