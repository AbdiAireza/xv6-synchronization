#include "types.h"
#include "stat.h"
#include "user.h"

#define ITEMS 5

#define PRODCONS_NUM 3

void safe_print(int is_producer, int id, int val) {
    char buf[64];
    int i = 0;
    char *str1 = is_producer ? "producer " : "consumer ";
    char *str2 = is_producer ? " produced " : " consumed ";
    

    while(*str1) buf[i++] = *str1++;
    

    buf[i++] = id + '0'; 
    
    
    while(*str2) buf[i++] = *str2++;
    
    
    char num[10];
    int k = 0;
    int temp = val;
    while(temp > 0) {
        num[k++] = (temp % 10) + '0';
        temp /= 10;
    }
    
    while(k > 0) {
        buf[i++] = num[--k];
    }
    
    buf[i++] = '\n';
    
    
    write(1, buf, i);
}
void producer(int id, int start) {
    for (int i = 0; i < ITEMS; i++) {\
        produce(start + i);
        safe_print(1, id, start + i);
    }
    exit();
}

void consumer(int id) {
    int consumed = 0;
    while (consumed < ITEMS) {
        int val = consume();
        safe_print(0, id, val);
        consumed++;
    }
    exit();
}

int main(int argc, char *argv[]) {
    for (int i = 1; i < PRODCONS_NUM + 1; i++){
        if (fork() == 0) producer(i, i * 1000);
    }

    for (int i = 1; i < PRODCONS_NUM + 1; i++){
        if (fork() == 0) consumer(i + PRODCONS_NUM);
    }


    for(int i = 0; i < 2 * PRODCONS_NUM; i++) {
        wait();
    }
    
    exit();
}