#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

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
    
    int lower_bound;
    int upper_bound;
    int n;

    int shmid;

    scanf("%d %d %d", lower_bound, upper_bound, n);

    // TODO shmid = shmget();


}
