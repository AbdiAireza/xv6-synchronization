#include "types.h"
#include "stat.h"
#include "user.h"

int main(void) {
    printf(1, "Starting Readers Writers Lock Test \n");
    rwinit();

    if(fork() == 0) {
        printf(1, "Reader 1: Requesting lock \n");
        rwacquireread();
        printf(1, "Reader 1: Entered Critical Section\n");
        sleep(100); 
        printf(1, "Reader 1: Leaving Critical Section\n");
        rwreleaseread();
        exit();
    }
    
    if(fork() == 0) {
        printf(1, "Reader 2: Requesting lock\n");
        rwacquireread();
        printf(1, "Reader 2: Entered Critical Section\n");
        sleep(100);
        printf(1, "Reader 2: Leaving Critical Section\n");
        rwreleaseread();
        exit();
    }

    sleep(20);

    if(fork() == 0) {
        printf(1, "Writer: Requesting lock\n");
        rwacquirewrite();
        printf(1, "Writer: Entered Critical Section\n");
        sleep(50);
        printf(1, "Writer: Leaving Critical Section\n");
        rwreleasewrite();
        exit();
    }

    sleep(10);

    if(fork() == 0) {
        printf(1, "Reader 3 (Late): Requesting lock\n");
        rwacquireread();
        printf(1, "Reader 3 (Late): Entered Critical Section\n");
        rwreleaseread();
        exit();
    }

    wait();
    wait();
    wait();
    wait();

    printf(1, "Test Finished \n");
    exit();
}