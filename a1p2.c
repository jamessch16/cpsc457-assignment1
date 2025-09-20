#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <sys/wait.h>
#include <sys/shm.h>

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

void search_primes_in_range(int lower_bound, int upper_bound, int *num_ptr) {
    /*
    Finds all the primes in the range [lower_bound, upper_bound] and writes them
    to the memory block marked by *num_ptr

    args:
    lower_bound: the lowest bound of the search range inclusive
    upper_bound: the upper bound of the search range inclusive
    num_ptr: an int pointer pointing to a block in memory where any found prime numbers should be stored
    */

    int primes_found = 0;

    for (int num = lower_bound; num <= upper_bound; num++) {
        if (is_prime(num)) {
            *num_ptr = num;
            num_ptr++;
            primes_found++;
        }
    }

    // Set a 'null char' of -1 to mark the end of prime numbers in this section if there is room left in the array
    if (primes_found < upper_bound - lower_bound + 1)  *num_ptr = -1;
}

void report_primes(int *num_ptr, int size, int n_children, int partition_size) {
    /*
    Print all stored prime numbers in the block in memory pointed to by num_ptr

    args:
    num_ptr: beginning of the memory block
    size: the size of the memory block
    offset: offset of each array dedicated to a child
    */

    for (int child = 0; child < n_children; child++) {

        for (int i = 0; i < partition_size; i++) {

            // move to nex process if reach end of row character
            if ( *(num_ptr + i) == -1) {
                break;
            }

            printf("%d ", *(num_ptr + i));
        }
        
        num_ptr += partition_size;
    }
}

int main(int argc, char *argv[]) {
    
    // input and numerical computation
    const int GLOBAL_LOWER_BOUND = atoi(argv[1]);
    const int GLOBAL_UPPER_BOUND = atoi(argv[2]);
    const int N_CHILDREN = atoi(argv[3]);

    if (GLOBAL_LOWER_BOUND < 0 || GLOBAL_UPPER_BOUND < 0 || N_CHILDREN <= 0) {
        fprintf(stderr, "ERROR: invalid input. Only non-negative numbers permitted for bounds and only positive integers for number of children");
        exit(1);
    }

    if (GLOBAL_LOWER_BOUND > GLOBAL_UPPER_BOUND) {
        fprintf(stderr, "ERROR: invalid input. Only lower bound must be greater than or equal to upper bound");
        exit(1);
    }

    int pid = -1;
    int counter = 0;

    int numbers_to_search = GLOBAL_UPPER_BOUND - GLOBAL_LOWER_BOUND + 1;
    int partition_size = -1;
    int remainder_size = -1;

    // calculate partition sizes
    if (numbers_to_search < N_CHILDREN) {  // there are more child processes than numbers to search
        partition_size = 1;
        remainder_size = 0;  // here remainder size is excess processes
    }
    else if (numbers_to_search % N_CHILDREN == 0) {   // partitions divide search space evenly
        partition_size = numbers_to_search / N_CHILDREN;
        remainder_size = 0;
    }
    else {  // partitions do not divide evenly
        partition_size = numbers_to_search / (N_CHILDREN - 1);  // one process is set aside to check remainder numbers
        remainder_size = numbers_to_search % N_CHILDREN;
    }

    int lower_bound = GLOBAL_LOWER_BOUND;
    int upper_bound = lower_bound + partition_size - 1;


    // setup shared memory
    const int MEM_SIZE = sizeof(int) * partition_size * N_CHILDREN;
    int shmid = shmget(IPC_PRIVATE, MEM_SIZE, IPC_CREAT | 0666);  // NOTE FOR FUTURE REFERENCE: IPC_PRIVATE is a flag to create a new segment. Don't use if unrelated processes need to share memory
    const int *SHM_PTR = (int *) shmat(shmid, NULL, 0);   
    int *num_ptr = (int *) SHM_PTR;

    // create child processes. loop exits early if process is a child
    while (counter < N_CHILDREN && counter < numbers_to_search && pid != 0) {

        pid = fork();

        // process is the parent
        if (pid > 0) {
            printf("Child PID %d checking range [%d, %d]\n", pid, lower_bound, upper_bound);

            // adjust search bounds for next child and its starting point in memory
            if ((counter < N_CHILDREN - 2) || remainder_size == 0)  {
                printf("counter%d\n", counter);  
                upper_bound += partition_size;
                lower_bound += partition_size;
                num_ptr += partition_size; 
            }
            else if (counter == N_CHILDREN - 2 && remainder_size > 0)  { 
                printf("set to global\n"); 
                upper_bound = GLOBAL_UPPER_BOUND;
                lower_bound += partition_size;
                num_ptr += partition_size;
            }
            
            counter++;
        }
    }


    // is parent
    if (pid > 0) {

        // wait for all children to finish. when all children are terminated, it will return -1 and the loop whill end
        while (wait(NULL) != -1) {}

        printf("\nParent: All children finished. Primes found:");
        report_primes(num_ptr, MEM_SIZE, N_CHILDREN, partition_size);
        shmdt(SHM_PTR);
    }
    // is child
    else if (pid == 0) {
        search_primes_in_range(lower_bound, upper_bound, num_ptr);
        shmdt(SHM_PTR);
    } 

    else {
        fprintf(stderr, "ERROR: Should never happen. Exit process creation with negative value from fork");
        shmdt(SHM_PTR);
        exit(1);
    }




    // TODO CASE: MORE PROCESSES THAN NUMBERS TO SEARCH



    // clean up memory
    shmctl (shmid, IPC_RMID, NULL);

    return 0;
}
