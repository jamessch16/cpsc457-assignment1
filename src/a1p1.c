#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    
    pid_t child_processes[100];
    pid_t pid = -1;

    // create child processes
    for (int i = 0; i < 100 && pid != 0; i++) {

        pid = fork();

        if (pid > 0) child_processes[i] = pid;
        else if (pid < 0) {
            i--;
            wait(10);
        }
    }

    // TODO What if children finish before wait starts???
    if (pid == 0) 1 == 1; // TODO Func call here
    else wait(NULL);


    return 0;
}