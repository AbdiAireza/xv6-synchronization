#include "types.h"
#include "stat.h"
#include "user.h"

#define NUM_PROCESSES 5
#define DELAY 5  

void delay(int ticks) {
    int start = uptime();
    while(uptime() - start < ticks) {
    }
}

void critical_section() {
    for(int i = 0; i < 1000000; i++) {
        int x = i * i;
        (void)x;
    }
}

int main(void) {
    int pid;
    int i;

    for(i = 1; i <= NUM_PROCESSES; i++) {
        pid = fork();
        
        if(pid == 0) {
            
            ticket_acquire();
            
            critical_section();
            
            ticket_release();
            exit();
        } else if(pid < 0) {
            printf(1, "Fork failed!\n");
            exit();
        }
        delay(DELAY);
    }

    for(i = 0; i < NUM_PROCESSES; i++) {
        wait();
    }

    exit();
}