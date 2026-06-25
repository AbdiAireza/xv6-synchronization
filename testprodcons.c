#include "types.h"
#include "stat.h"
#include "user.h"

void producer(int id, int start, int end) {
    for (int i = start; i <= end; i++) {
        while (produce(i) == -1) {
            
            sleep(1); 
        }
        printf(1, "producer %d produced %d\n", id, i);
    }
    exit();
}

void consumer(int id, int count) {
    for (int i = 0; i < count; i++) {
        int val;
        while ((val = consume()) == -1) {
            
            sleep(1);
        }
        printf(1, "consumer %d consumed %d\n", id, val);
    }
    exit();
}

int main(int argc, char *argv[]) {
    
    if (fork() == 0) producer(1, 100, 150);
    if (fork() == 0) producer(2, 200, 250);

    
    if (fork() == 0) consumer(1, 51);
    if (fork() == 0) consumer(2, 51);

    wait(); wait(); wait(); wait();
    exit();
}