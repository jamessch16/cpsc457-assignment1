#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>



/*
Notes on partitioning

Numbers to check = upper - lower + 1

if (to_check % buckets == 0) partition_size =  

*/

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
    
    // input and numerical computation
    int global_lower_bound = -1;
    int global_upper_bound = -1;
    int n_children = -1;

    scanf("%d %d %d", lower_bound, upper_bound, n);

    int pid = -1;
    int counter = 0;

    int numbers_to_search = global_upper_bound - global_lower_bound + 1;
    int partition_size = -1;
    int remainder_size = -1;

    // calculate partition sizes
    if (numbers_to_search < n_children) {  // there are more child processes than numbers to search
        partition_size = 1;
        remainder_size = n_children - numbers_to_search;  // here remainder size is excess processes
    }
    else if (numbers_to_search % n_children == 0) {   // buckets divide evenly
        partition_size = numbers_to_search / n_children;
        remainder_size = 0;
    }
    else {  // buckets do not divide evenly
        partition_size = numbers_to_search / (n_children - 1);  // one process is set aside to check remainder numbers
        remainder_size = numbers_to_search % n_children;
    }

    int lower_bound = global_lower_bound;
    int upper_bound = lower_bound + partition_size - 1;



    // setup shared memory
    int shmid = shmget(IPC_PRIVATE, sizeof(int) * numbers_to_search, IPC_CREAT | 0666);  // NOTE FOR FUTURE REFERENCE: IPC_PRIVATE is a flag to create a new segment. Don't use if unrelated processes need to share memory
    int *shm_ptr = (int *) shmat(shmid, NULL, 0);

    // create child processes. loop exits early if process is a child
    while (counter < n_children && pid != 0) {

        pid = fork();

        // process is the parent
        if (pid > 0) {
            printf("Child PID %d checking range [%d, %d]\n", pid, lower_bound, upper_bound);

            // adjust search bounds for next child
            lower_bound += partition_size;

            if ((counter < n_children - 1) || remainder_size == 0)  upper_bound += partition_size;
            else if (counter == n_children - 1)                     upper_bound = global_upper_bound;

            else {
                fprintf(stderr, "ERROR: Should never happen, exceed bounds of child process creation");
                exit(1);
            }
            
            counter++;
        }
    }


    // is parent
    if (pid > 0) {

    }
    // is child
    else if (pid == 0) {

    } 

    else {
        fprintf(stderr, "ERROR: Should never happen, exit process creation with negative value from fork");
        exit(1);
    }




    // TODO CASE: MORE PROCESSES THAN NUMBERS TO SEARCH



    // clean up memory
    shmctl (shmid, IPC_RMID, NULL);

    return 0;
}
