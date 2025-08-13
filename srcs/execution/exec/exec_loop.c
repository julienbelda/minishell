#include "minishell.h"
#include <unistd.h>
#include <stdio.h>

static int	open_pipe_if_needed(t_command *c, int pipefd[2])
{
	if (!c->next)
		return (0);
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return (1);
	}
	return (0);
}

/* static int	exec_pipeline_cmd(t_command *cmd, \
	t_minishell *ms, int *in_fd, int pipefd[2])
{
	int		out_fd;
	pid_t	pid;

	if (!handle_assignments(cmd, ms))
		return (0);
	if (open_pipe_if_needed(cmd, pipefd))
		return (1);
	if (cmd->next)
		out_fd = pipefd[1];
	else
		out_fd = STDOUT_FILENO;
	if (is_builtin(cmd) && !cmd->next && *in_fd == STDIN_FILENO && !cmd->redir)
	{
		ms->last_status = run_builtin(cmd, ms);
		return (0);
	}
	pid = launch_child(cmd, *in_fd, out_fd, ms);
	if (pid == -1)
		return (1);
	if (cmd->next)
		close(pipefd[1]);
	if (cmd->next && *in_fd != STDIN_FILENO)
		close(*in_fd);
	if (cmd->next)
		*in_fd = pipefd[0];
	else
		*in_fd = STDIN_FILENO;
	return (0);
} */

int	get_out_fd(t_command *cmd, int pipefd[2])
{
	if (cmd->next)
		return (pipefd[1]);
	return (STDOUT_FILENO);
}

int	run_builtin_parent(t_command *cmd, t_minishell *ms, int *in_fd)
{
	if (is_builtin(cmd) && !cmd->next && *in_fd == STDIN_FILENO && !cmd->redir)
	{
		ms->last_status = run_builtin(cmd, ms);
		return (1);
	}
	return (0);
}

void	update_in_fd(t_command *cmd, int *in_fd, int pipefd[2])
{
	if (cmd->next)
		*in_fd = pipefd[0];
	else
		*in_fd = STDIN_FILENO;
}

int	exec_pipeline_cmd(t_command *cmd, \
	t_minishell *ms, int *in_fd, int pipefd[2])
{
	pid_t	pid;
	int		out_fd;

	if (!handle_assignments(cmd, ms))
		return (0);
	if (open_pipe_if_needed(cmd, pipefd))
		return (1);
	out_fd = get_out_fd(cmd, pipefd);
	if (run_builtin_parent(cmd, ms, in_fd))
		return (0);
	pid = launch_child(cmd, *in_fd, out_fd, ms);
	if (pid == -1)
		return (1);
	if (cmd->next)
		close(pipefd[1]);
	if (cmd->next && *in_fd != STDIN_FILENO)
		close(*in_fd);
	update_in_fd(cmd, in_fd, pipefd);
	return (0);
}
