#include "types.h"
#include "stat.h"
#include "user.h"
#include "syscall.h" 

#define CHILDREN 4        
#define ITERATIONS 100000 

int main(int argc, char *argv[]) {
    int i, j;
    int pid;

    printf(1, "Starting race condition test with %d children, each calling getpid() %d times...\n", CHILDREN, ITERATIONS);

    // ساخت پردازه‌های فرزند
    for (i = 0; i < CHILDREN; i++) {
        pid = fork();
        
        if (pid < 0) {
            printf(1, "Fork failed!\n");
            exit();
        } 
        
        if (pid == 0) {
            
            for (j = 0; j < ITERATIONS; j++) {
                getpid();
            }
            exit(); 
        }
    }

    
    for (i = 0; i < CHILDREN; i++) {
        wait();
    }

    
    int actual_count = getcount(SYS_getpid); 
    int expected_count = CHILDREN * ITERATIONS;

    printf(1, "\n--- Test Results ---\n");
    printf(1, "Expected total calls: %d\n", expected_count);
    printf(1, "Actual counted calls: %d\n", actual_count);

    if (actual_count == expected_count) {
        printf(1, "Result: NO Race Condition (Counts match perfectly).\n");
    } else {
        printf(1, "Result: RACE CONDITION DETECTED! Lost %d updates.\n", expected_count - actual_count);
    }

    exit();
}