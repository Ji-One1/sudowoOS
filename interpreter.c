#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shellmemory.h"
#include "shell.h"
#include "unistd.h"
#include <dirent.h>
#include "utils.h"
#include <sys/stat.h>


int MAX_ARGS_SIZE = 7;

int badcommand() {
    printf("Unknown Command\n");
    return 1;
}

int badCommandTooManyArgs() {
    printf("Bad command: Too many arguments\n");
    return 2;
}

// For run command only
int badcommandFileDoesNotExist() {
    printf("Bad command: File not found\n");
    return 3;
}

int badcommandCustom(char *message) {
    printf("Bad command: %s\n", message);
    return 4;
}

int failedExecution() {
    printf("Execution Failed");
    return 5;
}

int help();
int quit();
int set(char *var, char *values[], int args_size);
int print(char *var);
int echo(char *var);
int run(char *script);
int my_ls();
int my_mkdir(char *dirname);
int my_touch(char *filename);
int my_cd(char *dirname);
int badcommandFileDoesNotExist();

// Interpret commands and their arguments
int interpreter(char* command_args[], int args_size) {
    int i;

    if (args_size < 1) {
        return badcommand();
    } else if (args_size > MAX_ARGS_SIZE) {
        return badCommandTooManyArgs();
    }

    // Terminate args at newline chars
    for (i = 0; i < args_size; i++) {
        command_args[i][strcspn(command_args[i], "\r\n")] = 0;
    }

    if (strcmp(command_args[0], "help") == 0){
        //help
        if (args_size != 1) return badcommand();
        return help();

    } else if (strcmp(command_args[0], "quit") == 0) {
        //quit
        if (args_size != 1) return badcommand();
        return quit();

    } else if (strcmp(command_args[0], "set") == 0) {
        return set(command_args[1], command_args, args_size);

    } else if (strcmp(command_args[0], "print") == 0) {
        if (args_size != 2) return badcommand();
        return print(command_args[1]);

    } else if (strcmp(command_args[0], "echo") == 0) {
        if (args_size != 2) return badcommand();
        return echo(command_args[1]);

    } else if (strcmp(command_args[0], "run") == 0) {
        if (args_size != 2) return badcommand();
        return run(command_args[1]);

    } else if (strcmp(command_args[0], "my_ls") == 0) {
        if (args_size != 1) return badcommand();
        return my_ls();

    } else if (strcmp(command_args[0], "my_mkdir") == 0) {
        if (args_size != 2) return badcommand();
        return my_mkdir(command_args[1]);

    } else if (strcmp(command_args[0], "my_touch") == 0) {
        if (args_size != 2) return badcommand();
        return my_touch(command_args[1]);
    } else if (strcmp(command_args[0], "my_cd") == 0) {
    if (args_size != 2) return badcommand();
    return my_cd(command_args[1]); 

    } else return badcommand();
}

int help() {

    // note the literal tab characters here for alignment
    char help_string[] = "COMMAND			DESCRIPTION\n \
help			Displays all the commands\n \
quit			Exits / terminates the shell with “Bye!”\n \
set VAR STRING		Assigns a value to shell memory\n \
print VAR		Displays the STRING assigned to VAR\n \
run SCRIPT.TXT		Executes the file SCRIPT.TXT\n ";
    printf("%s\n", help_string);
    return 0;
}

int quit() {
    printf("Bye!\n");
    exit(0);
}

int set(char *var, char *values[], int args_size) {

    char value[MAX_USER_INPUT] = "";

    for (int i = 2; i < args_size; i++) {
        strcat(value, values[i]);
        if (i < args_size - 1){
            strcat(value, " ");
        }
    }
    mem_set_value(var, value);

    return 0;
}

int print(char *var) {
    printf("%s\n", mem_get_value(var));
    return 0;
}

int echo(char *var) {
    if (var[0] != '$') {
        printf("%s\n", var);
        return 0;
    }
    char *val = mem_get_value(var + 1);
    if (strcmp(val, "Variable does not exist") == 0) {
        printf("should not run\n");
        val = "";
    }

    printf("%s\n", val);
    return 0;
}

int run(char *script) {
    int errCode = 0;
    char line[MAX_USER_INPUT];
    FILE *p = fopen(script, "rt");  // the program is in a file

    if (p == NULL) {
        return badcommandFileDoesNotExist();
    }

    fgets(line, MAX_USER_INPUT-1, p);
    while (1) {
        errCode = parseInput(line);	// which calls interpreter()
        memset(line, 0, sizeof(line));

        if (feof(p)) {
            break;
        }
        fgets(line, MAX_USER_INPUT-1, p);
    }

    fclose(p);

    return errCode;
}

int my_ls() {
    char **array = NULL;
    int capacity = 10;
    int count = 0;

    array = malloc(capacity * sizeof(char*));
    if (!array) return failedExecution();

    DIR *dir = opendir(".");
    if (!dir) return failedExecution();
    
    struct dirent *entry;
    // get filenames 
    while ((entry = readdir(dir)) != NULL) {
        //skip hidden files
        if (entry->d_name[0] == '.') continue;

        if (count == capacity) {
            capacity *= 2;
            array = realloc(array, capacity * sizeof(char*));
            if (array == NULL) {
                free(array);
                return failedExecution();
            }
        }
        
        array[count++] = strdup(strdup(entry->d_name));
    }

    closedir(dir);


    qsort(array, count, sizeof(char*), filename_comparator);

    for (int i = 0; i  < count; i++) {
        printf("%s\n", array[i]);
    }

    return 0;
}

int my_mkdir(char *dirname) {
    int var = 0; //0 if variable, 1 if not
    if (dirname[0] == '$') {
        dirname++;
        var = 1;
    }

    if (var) {
        char *val = mem_get_value(dirname);
        if (strcmp(val, "Variable does not exist") == 0) {
            return badcommandCustom("my_mkdir");
        }
        dirname = val;
    }
    if (!is_alphanumeric(dirname)) return badcommandCustom("my_mkdir");

    //0755 are perm bits
    mkdir(dirname, 0755);
    return 0;
}

int my_touch(char *filename) {

    if (!is_alphanumeric(filename)) return badcommandCustom("my_touch");
 
    FILE *file = fopen(filename, "a");
    fclose(file);
    return 0;
}

int my_cd(char *dirname) {
    if (!is_alphanumeric(dirname) && dirname[0] != '.') return badcommandCustom("my_cd");
 
    if (chdir(dirname) == 0) {
        return 0;
    } 
    return badcommandCustom("my_cd"); 
}

