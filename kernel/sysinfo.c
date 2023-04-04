#include "types.h"
#include "riscv.h"
#include "param.h"
#include "spinlock.h"
#include "defs.h"
#include "sysinfo.h"
#include "proc.h"


// Get the current system info.
int systeminfo(uint64 addr)
{
    struct proc* p = myproc();
    struct sysinfo info;
    info.freemem = freemem();
    info.nproc = nproc();
    if (copyout(p->pagetable, addr, (char*)&info, sizeof(info)) < 0)return -1;
    return 0;
}