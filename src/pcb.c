#include <stdlib.h>
#include "pcb.h"

PCB* ready_head = NULL;

PCB* init_PCB( int start_address, int length, int pc) {
    PCB *process = (PCB *)malloc(sizeof(PCB));
    if (process == NULL) {
        return NULL; 
    }
    process->pid = generate_pid();
    process->start_address = start_address;
    process->length = length;
    process->pc = pc;
    process->next = NULL;
    return process;
}

int generate_pid() {
    int i = 0;
    PCB* current = ready_head;
    while (current != NULL) {
        current = current->next;
        i++;
    }
    return i;
}

void deinit_PCB(PCB *process) {
    if (process != NULL) {
        free(process);
    }
}

void enqueue(PCB* process) {
    process->next = NULL;
    if (ready_head == NULL) {
        ready_head = process;
        return;
    }

    PCB* prev = ready_head;
    while (prev->next != NULL) {
        prev = prev->next;
    }
    prev->next = process;
}

PCB* dequeue() {
    if (ready_head == NULL) return NULL;

    PCB* process = ready_head;
    ready_head = ready_head->next;

    return process;
}
