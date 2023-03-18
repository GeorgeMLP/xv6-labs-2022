#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


void sieve(int left_pipegate[2])
{
    int num, buf, right_pipegate[2], pid;

    close(left_pipegate[1]);
    read(left_pipegate[0], &num, sizeof(num));
    if (num == -1)
    {
        close(left_pipegate[0]);
        exit(0);
    }
    printf("prime %d\n", num);

    if (pipe(right_pipegate) < 0)
    {
        fprintf(2, "pipe: cannot create pipe\n");
        exit(-1);
    }

    if ((pid = fork()) < 0)
    {
        fprintf(2, "fork: cannot fork process\n");
        exit(-1);
    }
    else if (pid == 0)  // child
    {
        close(left_pipegate[0]);
        sieve(right_pipegate);
    }
    else  // parent
    {
        close(right_pipegate[0]);
        while (read(left_pipegate[0], &buf, sizeof(buf)) && buf > 0)
            if (buf % num)
                write(right_pipegate[1], &buf, sizeof(buf));
        buf = -1;
        write(right_pipegate[1], &buf, sizeof(buf));
        close(right_pipegate[1]);
        close(left_pipegate[0]);
        wait(0);
        exit(0);
    }
}


int main(int argc, const char* argv[])
{
    int starting_pipegate[2], buf = -1;

    if (pipe(starting_pipegate) < 0)
    {
        fprintf(2, "pipe: cannot create pipe\n");
        exit(-1);
    }
    for (int i = 2; i <= 35; i++)
        write(starting_pipegate[1], &i, sizeof(i));
    write(starting_pipegate[1], &buf, sizeof(buf));
    sieve(starting_pipegate);
    exit(0);
}
