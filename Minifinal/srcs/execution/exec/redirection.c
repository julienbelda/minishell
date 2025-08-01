#include "minishell.h"

int	redir_in(const char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd < 0)
		return (perror(file), -1);
	if (dup2(fd, STDIN_FILENO) == -1)
		return (perror("dup2"), close(fd), -1);
	close(fd);
	return (0);
}

int	redir_in_fd(t_redirection *r)
{
	if (r->fd >= 0)
	{
		if (dup2(r->fd, STDIN_FILENO) == -1)
			return (perror("dup2"), -1);
		close(r->fd);
		return (0);
	}
	return (redir_in(r->file));
}

int	redir_out(const char *file)
{
	int	fd;

	fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return (perror(file), -1);
	if (dup2(fd, STDOUT_FILENO) == -1)
		return (perror("dup2"), close(fd), -1);
	close(fd);
	return (0);
}

int	redir_append(const char *file)
{
	int	fd;

	fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
		return (perror(file), -1);
	if (dup2(fd, STDOUT_FILENO) == -1)
		return (perror("dup2"), close(fd), -1);
	close(fd);
	return (0);
}

int	apply_redirections(t_redirection *redir)
{
	int	ret;

	ret = 0;
	while (redir)
	{
		if (redir->type == REDIR_IN)
			ret = redir_in_fd(redir);
		else if (redir->type == REDIR_OUT)
			ret = redir_out(redir->file);
		else if (redir->type == REDIR_APPEND)
			ret = redir_append(redir->file);
		if (ret < 0)
			return (-1);
		redir = redir->next;
	}
	return (0);
}
