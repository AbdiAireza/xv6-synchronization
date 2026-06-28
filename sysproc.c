#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "spinlock.h"
#include "sleeplock.h"


#define BUFFER_SIZE 1000

int buffer[BUFFER_SIZE];
int head = 0;
int tail = 0;
int count = 0;
struct sleeplock prodcons_lock;
struct spinlock print_lock;
struct ticketlock ticket_lock;


int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}


/*int
sys_getcount(void)
{
  int syscall_num;

  if(argint(0, &syscall_num) < 0)
    return -1;

  if(syscall_num <= 0 || syscall_num >= 30)
    return -1;

  extern int syscall_count[];
  return syscall_count[syscall_num];
} part 1.1&2*/








int
sys_getcount(void)
{
  int syscall_num;
  if(argint(0, &syscall_num) < 0 || syscall_num <= 0 || syscall_num >= 30)
    return -1;

  extern int syscall_count_cpu[NCPU][30];
  int total = 0;
  
  
  for(int i = 0; i < NCPU; i++){
    total += syscall_count_cpu[i][syscall_num];
  }
  return total;
}


int
sys_getcpucount(void)
{
  int cpu_id, syscall_num;

  
  if(argint(0, &cpu_id) < 0 || argint(1, &syscall_num) < 0)
    return -1;

  if(syscall_num <= 0 || syscall_num >= 30 || cpu_id < 0 || cpu_id >= NCPU)
    return -1;

  extern int syscall_count_cpu[NCPU][30];
  return syscall_count_cpu[cpu_id][syscall_num];
}




int
sys_produce(void)
{

  int val;
  
  if(argint(0, &val) < 0){
    return -1;
  }

  acquiresleep(&prodcons_lock); 
  
  while(count == BUFFER_SIZE) {
    sleep(&prodcons_lock, &prodcons_lock.lk);
  }

  
  buffer[tail] = val;
  tail = (tail + 1) % BUFFER_SIZE;
  count++;

  releasesleep(&prodcons_lock); 
  return 0; 
}


int
sys_consume(void)
{
  int val;

  acquiresleep(&prodcons_lock); 
  
  while(count == 0) {
    sleep(&prodcons_lock, &prodcons_lock.lk);
  }

  
  val = buffer[head];
  head = (head + 1) % BUFFER_SIZE;
  count--;

  releasesleep(&prodcons_lock); 
  
  return val; 
}
struct rwlock test_lock;

int sys_rwinit(void) {
  rwlock_init(&test_lock);
  return 0;
}

int sys_rwacquireread(void) {
  rwlock_acquire_read(&test_lock);
  return 0;
}

int sys_rwreleaseread(void) {
  rwlock_release_read(&test_lock);
  return 0;
}

int sys_rwacquirewrite(void) {
  rwlock_acquire_write(&test_lock);
  return 0;
}

int sys_rwreleasewrite(void) {
  rwlock_release_write(&test_lock);
  return 0;
}

int
sys_lock_print(void)
{
    char str[128];
    int i;
    char *user_str;
    
    if(argptr(0, (void*)&user_str, sizeof(char*)) < 0)
        return -1;
    
    for(i = 0; i < sizeof(str) - 1; i++) {
        if(user_str[i] == '\0') break;
        str[i] = user_str[i];
    }
    str[i] = '\0';
    
    acquire(&print_lock);
    cprintf("%s", str); 
    release(&print_lock);
    
    return 0;
}

int sys_ticket_acquire(void) {
    ticketlock_acquire(&ticket_lock);
    return 0;
}

int sys_ticket_release(void) {
    ticketlock_release(&ticket_lock);
    return 0;
}