#include "minishell.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int count_commands(t_command *cmds)
{
	int count = 0;
	while (cmds)
	{
		count++;
		cmds = cmds->next;
	}
	return count;
}


void setup_child(t_command *cmds, t_minishell *mini, int pipe_fd[], int prev_fd)
{
    if (prev_fd != -1)
        dup2(prev_fd, STDIN_FILENO);
    if (cmds->next)
        dup2(pipe_fd[1], STDOUT_FILENO);
    if (cmds->next)
    {
        close(pipe_fd[0]);
        close(pipe_fd[1]);
    }
    if (prev_fd != -1)
        close(prev_fd);
    execvp(cmds->args[0], cmds->args);
    perror("execvp");
    exit(1);
}

int ft_execute(t_command *cmds, t_minishell *mini)
{
	int pipe_fd[2];
	int prev_fd = -1;
	pid_t pid;

	while (cmds)
	{
		// Créer un pipe si commande suivante
		if (cmds->next && pipe(pipe_fd) == -1)
		{
			perror("pipe");
			return 1;
		}

		pid = fork();
		if (pid < 0)
		{
			perror("fork");
			return 1;
		}

		if (pid == 0)
            setup_child(cmds, mini, pipe_fd, prev_fd);
		// Parent
		if (prev_fd != -1)
			close(prev_fd);
		if (cmds->next)
			close(pipe_fd[1]);

		prev_fd = cmds->next ? pipe_fd[0] : -1;
		cmds = cmds->next;
	}

	// Attend tous les enfants
	while (wait(NULL) > 0)
		;

	return 0;
}

/* int main(void)
{
	t_minishell mini;
	t_command cmd1, cmd2;

	mini.env = NULL;
	mini.last_status = 0;

	char *args1[] = {"echo", "hello", "world", NULL};
	cmd1.name = args1[0];
	cmd1.args = args1;
	cmd1.redirection = NULL;
	cmd1.prev = NULL;
	cmd1.next = &cmd2;

	char *args2[] = {"wc", "-c", NULL};
	cmd2.name = args2[0];
	cmd2.args = args2;
	cmd2.redirection = NULL;
	cmd2.prev = &cmd1;
	cmd2.next = NULL;

	ft_execute(&cmd1, &mini);
	return 0;
}
 */