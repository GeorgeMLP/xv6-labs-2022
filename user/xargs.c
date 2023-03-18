#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/param.h"


int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        fprintf(2, "usage: xargs <command>\n");
        exit(1);
    }
    if (argc + 1 > MAXARG)
    {
        fprintf(2, "xargs: too many arguments\n");
        exit(1);
    }

    char *command = argv[1], *params[MAXARG], buf[512];
    for (int i = 0; i < argc - 1; i++)params[i] = argv[i + 1];
    params[argc] = '\0';

    int i = 0, pid;
    while (read(0, &buf[i], 1))
        if (buf[i] != '\n')i++;
        else
        {
            buf[i] = '\0';
            i = 0;
            params[argc - 1] = buf;
            if ((pid = fork()) < 0)
            {
                fprintf(2, "fork: cannot fork process\n");
                exit(-1);
            }
            else if (pid == 0)  // child
            {
                exec(command, params);
                exit(0);
            }
            else wait(0);
        }
    exit(0);
}
