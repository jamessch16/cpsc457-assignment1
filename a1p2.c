#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>

// int spawn_children(int n) {
//     /*
//     Creates 100 child processes to search the file rows.
    
//     Returns:
//     -1:                             if process exits function as the parent
//     row for the process to search:  if process exits function as the child
//     */

//     return -1;
// }


int is_prime(int num) {
    /*
    Tests if an number is prime. Taken from assignment page.

    arguments:
    num: the number to be tested

    returns 1 if the number is prime, 0 otherwise 
    */
    
    if (num < 2) return 0;
    for (int i = 2; i <= sqrt(num); i++) {
        if (num % i == 0) return 0;
    }

    return 1;
}

int main() {
    
    int lower_bound = -1;
    int upper_bound = -1;
    int n_children = -1;

    scanf("%d %d %d", lower_bound, upper_bound, n);

    int shmid = shmget(IPC_PRIVATE, SIZE, IPC_CREAT | 0666);  // NOTE FOR FUTURE REFERENCE: IPC_PRIVATE is a flag to create a new segment. Don't use if unrelated processes need to share memory
    int *shm_ptr = (int *) shmat(shmid, NULL, 0);

    int pid = -1;
    int counter = 0;

    // create child processes. loop exits early if process is a child
    while (counter < NUM_ROWS && pid != 0) {

        pid = fork();

        if (pid > 0) {
            printf("Child PID %d checking range [%d, %d]\n", pid, INT_MAX, INT_MIN);
            counter++;
        }
    }


    



    // clean up memory
    shmctl (shmid, IPC_RMID, NULL);

    return 0;
}
