#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

const int NUM_ROWS = 100;
const int NUM_COLUMNS = 1000;
const int TARGET_NOT_FOUND = 200;

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

    int current = 0;

    fseek(stdin, 2 * row_num * NUM_COLUMNS, SEEK_SET);

    for (int i = 0; i < NUM_COLUMNS; i++) {
        int suc = -1;
	    current = -((100 + i) % 26);
        suc = scanf("%d", &current);
        
        if (suc == 0) {
            fprintf(stderr, "ERROR: Non-numerical entry");
            exit(1);
        }	

        if (current == 1) {
            return i;
        }
    }

    return -1;
}

int main() {
    
    int pid = -1;
    int process_row = -1;
    int row_num = -1;
    int column_num = -1;
    int wait_status = TARGET_NOT_FOUND;
    int counter = 0;

    process_row = create_child_processes();

    // process is the parent
    if (process_row == -1) {
        
        // wait until a process exits with EXIT_SUCCESS
        do {
            pid = wait(&wait_status);
	        
            // find the target in column
            row_num = WEXITSTATUS(wait_status);
            column_num = search_row(row_num);

            // TODO replace with while (pid != -1 and test)

            counter++;
        
        } while (WEXITSTATUS(wait_status) == TARGET_NOT_FOUND && counter < NUM_ROWS + 1);

        // Catch input errors
        if (WEXITSTATUS(wait_status) == TARGET_NOT_FOUND) {
            fprintf(stderr, "ERROR: No target in file");
            exit(1);
        }

        if (column_num == -1) {
            fprintf(stderr, "READ ERROR - Please try again");
            exit(1);
        }

        // report
        printf("Parent: The treasure was found by child with PID %d at row %d and column %d", pid, row_num, column_num);
    }
    
    // process is a child
    else {
        column_num = search_row(process_row);
	
        if (column_num == -1)  exit(TARGET_NOT_FOUND);
        else  exit(process_row);  // return row number if success
    }

    return 0;
}
