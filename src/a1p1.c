#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

const int NUM_ROWS = 100;

int main() {
    
    pid_t child_processes[NUM_ROWS];
    pid_t pid = -1;

    // create child processes
    for (int i = 0; i < NUM_ROWS && pid != 0; i++) {

        pid = fork();

        // TODO May have to redo structure. wait(NULL) until child done then create next t

        if (pid > 0) {
            child_processes[i] = pid;
            printf("Child %d (PID %d): Searching row %d\n", i, pid, i);
        }

        if (pid < 0) {
            i--;
            wait(10);
        }
    }

    // TODO What if children finish before wait starts???
    if (pid == 0) 1 == 1; // TODO Func call here
    else wait(NULL);


    return 0;
}