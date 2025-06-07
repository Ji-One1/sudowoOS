#define MEM_SIZE 1000
#define VAR_TABLE_SIZE 20
void mem_init();
char *get_var_value(char *var);
void set_var_value(char *var, char *value);
int set_memory(char *script[], int script_size);
int free_memory(int address, int size);
void get_memory(int address, int size, char **script);
