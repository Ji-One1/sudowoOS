#include <string.h>
#include "shellmemory.h"

struct memory_struct {
    char *var;
    char *value;
};

struct memory_struct var_value_table[VAR_TABLE_SIZE];
char *shell_memory[MEM_SIZE];

// Helper functions
// int match(char *model, char *var) {
//     int i, len = strlen(var), matchCount = 0;

//     for (i = 0; i < len; i++) {
//         if (model[i] == var[i]) matchCount++;
//     }

//     if (matchCount == len) {
//         return 1;
//     } else {
//         return 0;
//     }
// }


// === Shell memory functions ===

void mem_init() {
    int i;
    for (i = 0; i < VAR_TABLE_SIZE; i++){
        var_value_table[i].var   = NULL;
        var_value_table[i].value = NULL;
    }

    for (int i = 0; i < MEM_SIZE; i++){
        shell_memory[i] = NULL;
    }
 }
 
// Set key value pair
void set_var_value(char *var_in, char *value_in) {
    int i;

    for (i = 0; i < MEM_SIZE; i++) {
        if (strcmp(var_value_table[i].var, var_in) == 0){
            var_value_table[i].value = strdup(value_in);
            return;
        }
    }

    // Value does not exist, need to find a free spot.
    for (i = 0; i < MEM_SIZE; i++) {
        if (var_value_table[i].var == NULL){
            var_value_table[i].var = strdup(var_in);
            var_value_table[i].value = strdup(value_in);
            return;
        }
    }

    return;
}

// Get value based on input key
char *get_var_value(char *var_in) {
    int i;

    for (i = 0; i < MEM_SIZE; i++) {
        if (strcmp(var_value_table[i].var, var_in) == 0) {
            return strdup(var_value_table[i].value);
        }
    }

    return "Variable does not exist";
}

int find_free_block(int size_needed) {
    int free_count = 0;

    for (int i = 0; i < MEM_SIZE; i++) {
        if (shell_memory[i] == NULL) {
            free_count++;
            if (free_count == size_needed) {
                return i - size_needed + 1;
            }
        } else {
            free_count = 0;
        }
    }

    return -1; // no suitable block found
}

int set_memory(char *script[], int script_size) {
    int free_address = find_free_block(script_size);
    if (free_address == -1) {
        return -1;
    }

    for (int i = 0; i < script_size; i++) {
        if (shell_memory[free_address + i] != NULL) {
            free(shell_memory[free_address + i]);
        }
        shell_memory[free_address + i] = strdup(script[i]);
        if (shell_memory[free_address + i] == NULL) {
            return -1;
        }
    }
    return 0;
}

void get_memory(int address, int size, char **script) {
    if ((address + size ) > MEM_SIZE) { return -1; }
    for (int i = 0; i < size; i++) {
        script[i] = strdup(shell_memory[i]);
    }
    return script;
}

int free_memory(int address, int size) {
    if (address < 0 || address + size > MEM_SIZE) {
        return -1;
    }

     for (int i = 0; i < size; i++) {
            if (shell_memory[address + i] != NULL) {
                free(shell_memory[address + i]);
                shell_memory[address + i] = NULL;
            }
     }
    return 0;
}