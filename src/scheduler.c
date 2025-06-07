#include "pcb.h"
#include "shellmemory.h"
#include "shell.h"
#include "scheduler.h"

int FCFS_execute_next_process() {
    PCB* process = dequeue();
    char *script[process->length];
    int err_code = get_memory(process->start_address, process->length, script);
    if (err_code != 0) { return -1; }
    execute_process(script, process->length);
    free_memory(process->start_address, process->length);
    deinit_PCB(process);
    return 0;
}


void execute_process(char **script, int size) {
    for (int i = 0; i < size; i++) {
        parseInput(script[i]);
    }
}   
