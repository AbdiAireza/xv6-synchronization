#include "types.h"
#include "stat.h"
#include "user.h"




int main(void) {
    lock_print("Starting Readers Writers Lock Test\n");
    rwinit();

    if(fork() == 0) {
        lock_print("Reader 1: Requesting lock\n");
        rwacquireread();
        lock_print("Reader 1: Entered Critical Section\n");
        sleep(100); 
        lock_print("Reader 1: Leaving Critical Section\n");
        rwreleaseread();
        exit();
    }
    
    if(fork() == 0) {
        lock_print("Reader 2: Requesting lock\n");
        rwacquireread();
        lock_print("Reader 2: Entered Critical Section\n");
        sleep(100);
        lock_print("Reader 2: Leaving Critical Section\n");
        rwreleaseread();
        exit();
    }

    sleep(20);

    if(fork() == 0) {
        lock_print("Writer: Requesting lock\n");
        rwacquirewrite();
        lock_print("Writer: Entered Critical Section\n");
        sleep(50);
        lock_print("Writer: Leaving Critical Section\n");
        rwreleasewrite();
        exit();
    }

    sleep(10);

    if(fork() == 0) {
        lock_print("Reader 3 (Late): Requesting lock\n");
        rwacquireread();
        lock_print("Reader 3 (Late): Entered Critical Section\n");
        rwreleaseread();
        exit();
    }

    wait();
    wait();
    wait();
    wait();

    lock_print("Test Finished \n");
    exit();
}