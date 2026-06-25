#include "types.h"
#include "stat.h"
#include "user.h"

void producer(int id, int start, int end) {
    for (int i = start; i <= end; i++) {
        
        while (produce(i) == -1) {
            sleep(1); 
        }
        printf(1, "producer %d produced %d\n", id, i);
        sleep(1); 
    }
    exit();
}

void consumer(int id, int count) {
    int consumed_items = 0;
    while (consumed_items < count) {
        int val = consume();
        if (val != -1) {
            
            printf(1, "consumer %d consumed %d\n", id, val);
            consumed_items++;
            sleep(1); 
        } else {
            
            sleep(1);
        }
    }
    exit();
}

int main(int argc, char *argv[]) {
    
    if (fork() == 0) producer(1, 100, 150);
    if (fork() == 0) producer(2, 200, 250);
    if (fork() == 0) producer(3, 300, 350);

    if (fork() == 0) consumer(1, 51);
    if (fork() == 0) consumer(2, 51);
    if (fork() == 0) consumer(3, 51);

    
    for(int i = 0; i < 6; i++) {
        wait();
    }
    
    exit();
}