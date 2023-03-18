#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


int main(int argc, const char* argv[])
{
    int pid, pipegate_p2c[2], pipegate_c2p[2];
    char buf = '\0';

    if (pipe(pipegate_p2c) < 0)
    {
        fprintf(2, "pipe: cannot create pipe\n");
        exit(-1);
    }
    if (pipe(pipegate_c2p) < 0)
    {
        fprintf(2, "pipe: cannot create pipe\n");
        exit(-1);
    }

    if ((pid = fork()) < 0)
    {
        fprintf(2, "fork: cannot fork process\n");
        exit(-1);
    }
    else if (pid > 0)  // parent
    {
        write(pipegate_p2c[1], "a", 1);
        read(pipegate_c2p[0], &buf, 1);
        printf("%d: received pong\n", getpid());
    }
    else  // child
    {
        read(pipegate_p2c[0], &buf, 1);
        printf("%d: received ping\n", getpid());
        write(pipegate_c2p[1], &buf, 1);
    }
    exit(0);
}