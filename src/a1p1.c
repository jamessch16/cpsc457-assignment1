#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

const int NUM_ROWS = 100;
const int NUM_COLUMNS = 1000;
const int TARGET_NOT_FOUND = 200;

// TODO TODO What if it is found before the all processes are spawned


int create_child_processes() {
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
            counter++;
        }
    }

    if (pid == 0) return counter;
    else return -1;
}

int search_row(int row_num) {
    /*
    Searches the row of the input specified by row_num

    args:
    row_num: the row to search

    returns: 
    the columns the target was found
    -1 if no target found
    */

    char current;

    fseek(stdin, 2 * row_num * NUM_COLUMNS, SEEK_CUR);

    for (int i = 0; i < NUM_COLUMNS; i++) {

        current = getc(stdin);

        if (current == '1') return i;
        
        fseek(stdin, 1, SEEK_CUR);
    }

    return -1;
}

int main() {
    
    pid_t pid = -1;
    int process_row = -1;
    int row_num = -1;
    int column_num = -1;
    int wait_status = TARGET_NOT_FOUND;
    int counter = 0;
    bool target_found = false;

    process_row = create_child_processes(child_processes);

    // process is the parent
    if (process_row == -1) {
        
        // wait until a process exits with EXIT_SUCCESS
        do {
            pid = wait(&wait_status);
            counter++;
        } while (WEXITSTATUS(wait_status) == TARGET_NOT_FOUND && counter < NUM_ROWS);     // TODO HANDLE NO TARGET IN INPUT

        // find the target in column
        row_num = wait_status;
        column_num = search_row(row_num);
        
        printf("Parent: The treasure was found by child with PID %d at row %d and column %d", pid, row_num, column_num);
    }
    
    // process is a child
    else {
        target_found = search_row(process_row);

        if (target_found == -1)     exit(INT_MAX);           // return full bitmask to indicate failure
        else                        exit(TARGET_NOT_FOUND);  // return row number if success
    }

    return 0;
}