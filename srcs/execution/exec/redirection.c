#include "minishell.h"

void redir_in(const char *file)
{
    int fd;
    fd = open(file, O_RDONLY);
    if (fd < 0)
    {
        perror(file);
        exit(1);
    }
    dup2(fd, STDIN_FILENO);
    close(fd);
}

void redir_out(const char *file)
{
    int fd;
    fd = open(file, O_WRONLY | O_TRUNC | O_CREAT , 0644);
    if (fd < 0)
    {
        perror(file);
        exit(1);
    }
    dup2(fd, STDOUT_FILENO);
    close(fd);
}

void redir_append(const char *file)
{
    int fd;
    fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd < 0)
    {
        perror(file);
        exit(1);
    }
    dup2(fd, STDOUT_FILENO);
    close(fd);
}

void apply_redir(t_command *cmds)
{
    t_redirection *redir;
    redir = cmds->redirection;
    while (redir)
    {
        if (redir->type == REDIR_IN)
            redir_in(redir->file);
        else if(redir->type == REDIR_OUT)
            redir_out(redir->file);
        else if (redir->type == REDIR_APPEND)
            redir_append(redir->file);
        redir = redir->next;
    }
}

/* int main(void)
{
	t_minishell mini;
	t_command cmd;
	t_redirection redir;
	char *args[] = {"echo", "hello", NULL};

	// Init de la mini structure
	mini.env = NULL;
	mini.last_status = 0;

	// Init de la commande
	cmd.name = "echo";
	cmd.args = args;
	cmd.redirection = &redir;
	cmd.next = NULL;
	cmd.prev = NULL;

	// Redirection > out.txt
	redir.type = REDIR_OUT;
	redir.file = "out.txt";
	redir.next = NULL;

	pid_t pid = fork();
	if (pid == 0)
	{
		apply_redir(&cmd); // redirige stdout vers out.txt
		execvp(cmd.args[0], cmd.args);
		perror("execvp");
		exit(1);
	}
	else
	{
		wait(NULL);
	}

	return 0;
}
 */
