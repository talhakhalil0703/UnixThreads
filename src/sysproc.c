#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

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

int
sys_join(void)
{
    //cprintf("Called system join\n");
    // void**
    int stack;
    if (argint(0 , &stack) < 0)
        return -1;

    //cprintf("Calling user join \n");
    return join((void **)stack);
}

int
sys_clone(void)
{
    //cprintf("Called system clone \n");
    // void(*)(void *, void*)
    int fnc;
    //void*
    int arg1;
    //void*
    int arg2;
    //void*
    int stack;

    // Read them as arguments and then cast these functions
    if(argint(0, &fnc) < 0)
        return -1;
    if(argint(1, &arg1) < 0)
        return -1;
    if(argint(2, &arg2) < 0)
        return -1;
    if(argint(3, &stack) < 0)
        return -1;

    //cprintf("Calling user clone\n");
    return clone((void(*)(void*, void*))fnc, (void *)arg1, (void *)arg2, (void *) stack);
}