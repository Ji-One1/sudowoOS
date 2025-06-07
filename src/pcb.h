#include <stdlib.h>
typedef struct PCB {
    int pid;
    int start_address;
    int length;
    int pc;
    struct PCB* next; // for queue
} PCB;

int generate_pid();
PCB* init_PCB(int start_address, int length, int pc);
void deinit_PCB(PCB *process);
extern PCB* ready_head;
void enqueue(PCB* process);
PCB* dequeue();
