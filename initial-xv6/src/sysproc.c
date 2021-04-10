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
sys_getusagecount(void)
{
  // parse both arguments as integers
  int is_reset_flag;
  int sys_call_to_follow;

  if (argint(0, &is_reset_flag) < 0 || argint(1, &sys_call_to_follow) < 0)
    return -1;

  // reset the counter
  if (is_reset_flag == 1) {
    acquire(&usage_count_lock);
    usage_counter_count = 0;
    release(&usage_count_lock);
  }

  // validate sys call range, not necessary
  if (sys_call_to_follow > 0 && sys_call_to_follow < sys_call_count)
  {
    // sys call to track differs from the previous value
    if (sys_call_to_follow != sys_call_usage_to_track)
    {
      // reset the counter if not already reset
      // and update the new call to follow
      if (!is_reset_flag)
      {
        acquire(&usage_count_lock);
        usage_counter_count = 0;
        release(&usage_count_lock);
      }
      sys_call_usage_to_track = sys_call_to_follow;
    }
  }
  else
  {
    return -1;
  }

  return usage_counter_count;
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
