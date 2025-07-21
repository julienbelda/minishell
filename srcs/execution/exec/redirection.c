#include "minishell.h"

int redir_in(const char *file)
{
    int fd = open(file, O_RDONLY);
    if (fd < 0)
        return (perror(file), -1);
    if (dup2(fd, STDIN_FILENO) == -1)
        return (perror("dup2"), close(fd), -1);
    close(fd);
    return 0;
}

int redir_out(const char *file)
{
    int fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0)
        return (perror(file), -1);
    if (dup2(fd, STDOUT_FILENO) == -1)
        return (perror("dup2"), close(fd), -1);
    close(fd);
    return 0;
}

int redir_append(const char *file)
{
    int fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd < 0)
        return (perror(file), -1);
    if (dup2(fd, STDOUT_FILENO) == -1)
        return (perror("dup2"), close(fd), -1);
    close(fd);
    return 0;
}

/* TODO: heredoc -> créer fichier tmp + expansion selon quotes */
int redir_heredoc(t_redirection *r)
{
    (void)r;
    return 0;
}

int apply_redirections(t_redirection *redir)
{
    while (redir)
    {
        int ret = 0;
        if (redir->type == REDIR_IN)
            ret = redir_in(redir->file);
        else if (redir->type == REDIR_OUT)
            ret = redir_out(redir->file);
        else if (redir->type == REDIR_APPEND)
            ret = redir_append(redir->file);
        else if (redir->type == HEREDOC)
            ret = redir_heredoc(redir);
        if (ret < 0)
            return -1;
        redir = redir->next;
    }
    return 0;
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
