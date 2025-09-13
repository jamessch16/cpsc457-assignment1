#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <unistd.h>

const int NUM_ROWS = 100;

bool created_child_processes(pid_t child_processes[]) {
    /*
    Creates 100 child processes to search the file rows.
    
    Returns:
    true: if process exits function as the parent
    false: if process exits function as the child
    */

    pid_t pid = -1;

    // create child processes. loop exits early if process is a child
    for (int i = 0; i < NUM_ROWS && pid != 0; i++) {

        pid = fork();

        if (pid > 0) {
            child_processes[i] = pid;
            printf("Child %d (PID %d): Searching row %d\n", i, pid, i);
        }

        if (pid < 0) {
            i--;
            wait(10);
        }
    }

    if (pid == 0) return false;
    else return 1;
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