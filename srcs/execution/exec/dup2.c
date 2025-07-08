#include <unistd.h>
#include <stdio.h>
#include "minishell.h"
#include <stdlib.h>

int main()
{
    pid_t pid;
    pid = fork();
    int fd;
    fd = open("texte.txt", O_RDWR | O_TRUNC | O_CREAT, 0744);
    if (pid == -1)
        printf("error pid");
    if (pid == 0)
    {
        printf("je suis l'enfant\n");
        printf("mon pid est %d\n", pid);
        dup2(fd, STDOUT_FILENO);
        printf("redirection pipe\n");
        printf("mon pid est %d\n", pid);

        // on est dans l'enfant
    }
    else
    {
        waitpid(pid, &pid, 0);
        printf("je suis le parent\n");
        printf("mon pid est %d\n", pid);
        // on est dans l'adulte
    }
}

