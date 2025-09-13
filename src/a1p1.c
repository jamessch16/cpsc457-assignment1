#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <unistd.h>

const int NUM_ROWS = 100;

int created_child_processes(pid_t child_processes[]) {
    /*
    Creates 100 child processes to search the file rows.
    
    Returns:
    -1:                             if process exits function as the parent
    row for the process to search:  if process exits function as the child
    */

    pid_t pid = -1;
    int counter = 0;

    // create child processes. loop exits early if process is a child
    while (counter < NUM_ROWS && pid != 0) {

        pid = fork();

        if (pid > 0) {
            child_processes[counter] = pid;
            printf("Child %d (PID %d): Searching row %d\n", counter, pid, counter);
        }

        // wait if process creating failed and reset counter
        if (pid < 0) {
            counter--;
            wait(10);
        }

        counter++;
    }

    if (pid == 0) return counter;
    else return -1;
}

void search_row(int row_num) {

}

int main() {
    
    pid_t child_processes[NUM_ROWS];
    pid_t parent_pid = getpid();
    pid_t pid;
    bool is_parent = true;

    is_parent = created_child_processes();

    // TODO WAIT

    // report PID of process that finds target
    for (int i = 0; i < NUM_ROWS; i++){
        if (child_processes[i] = pid) {
            
        }
    }

    return 0;
}