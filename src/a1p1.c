#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <unistd.h>

const int NUM_ROWS = 100;
const int NUM_COLUMNS = 1000;


// TODO TODO What if it is found before the all processes are spawned


int create_child_processes(pid_t child_processes[]) {
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

        
        // TODO TODO What if it is found before the all processes are spawned

        if (pid > 0) {
            child_processes[counter] = pid;
            printf("Child %d (PID %d): Searching row %d\n", counter, pid, counter);
            counter++;
        }

        // wait if process creating failed
        // if (pid < 0) {
        //     wait(10);
        // }
    }

    if (pid == 0) return counter;
    else return -1;
}

bool search_row(int row_num) {

    char current;
    bool found = false;

    fseek(stdin, 2 * row_num * NUM_COLUMNS, SEEK_CUR);

    for (int i = 0; i < NUM_COLUMNS && !found; i++) {

        current = getc(stdin);

        if (current == '1') found = true;
        
        fseek(stdin, 1, SEEK_CUR);
    }

    return found;
}

int main() {
    
    pid_t child_processes[NUM_ROWS];
    pid_t parent_pid = getpid();
    pid_t pid;
    int process_row;
    bool target_found = false;

    process_row = create_child_processes(child_processes);

    // process is the parent
    if (process_row == -1) {
        wait(NULL); // TODO HERE

        // report PID of process that finds target
        for (int i = 0; i < NUM_ROWS; i++) {
            if (child_processes[i] = pid) {

                // TODO TODO What if it is found before the all processes are spawned
                
            }
        }
    }
    
    // process is a child
    else {
        target_found = search_row(process_row);
    }



    return 0;
}