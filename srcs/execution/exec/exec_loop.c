#include "minishell.h"

static int	fd_save(t_fd *bak)
{
	bak->in = dup(STDIN_FILENO);
	bak->out = dup(STDOUT_FILENO);
	return (bak->in < 0 || bak->out < 0);
}

static void	fd_restore(t_fd *bak)
{
	dup2(bak->in, STDIN_FILENO);
	dup2(bak->out, STDOUT_FILENO);
	close(bak->in);
	close(bak->out);
}

static int	builtin_no_fork(t_command *cmd, t_minishell *ms)
{
	t_fd	bak;

	if (fd_save(&bak))
		return (perror("dup"), 1);
	if (apply_redirections(cmd->redir) == -1)
		return (fd_restore(&bak), 1);
	run_builtin(cmd, ms);
	fd_restore(&bak);
	return (0);
}

static int	open_pipe_if_needed(t_command *c, int pipefd[2])
{
	if (!c->next)
		return (0);
	if (pipe(pipefd) == -1)
		return (perror("pipe"), 1);
	return (0);
}

static int	builtin_if_single(t_command *c, int in_fd, t_minishell *ms)
{
	if (!c->next && in_fd == STDIN_FILENO && is_builtin(c))
	{
		builtin_no_fork(c, ms);
		return (1);
	}
	return (0);
}

static int	launch_and_check(t_command *c, int in_fd, int out_fd,
	t_minishell *ms)
{
	if (launch_child(c, in_fd, out_fd, ms) == -1)
		return (1);
	return (0);
}

static void	parent_fd_maintenance(int *in_fd, int pipefd[2], bool has_next)
{
	if (*in_fd != STDIN_FILENO)
		close(*in_fd);
	if (has_next)
	{
		close(pipefd[1]);
		*in_fd = pipefd[0];
	}
	else
		*in_fd = STDIN_FILENO;
}

static int	exec_pipeline(t_command *cmds, t_minishell *ms)
{
	int	pipefd[2];
	int	in_fd;

	in_fd = STDIN_FILENO;
	while (cmds)
	{
		if (!handle_assignments(cmds, ms))
		{
			cmds = cmds->next;
			continue ;
		}
		if (open_pipe_if_needed(cmds, pipefd))
			return (1);
		if (builtin_if_single(cmds, in_fd, ms))
			return (ms->last_status);
		if (launch_and_check(cmds, in_fd,
				cmds->next ? pipefd[1] : STDOUT_FILENO, ms))
			return (1);
		parent_fd_maintenance(&in_fd, pipefd, cmds->next != NULL);
		cmds = cmds->next;
	}
	return (0);
}

static void	wait_all(t_minishell *ms)
{
	int	status;

	while (waitpid(-1, &status, 0) > 0)
		if (WIFEXITED(status))
			ms->last_status = WEXITSTATUS(status);
}

int	ft_execute(t_command *cmds, t_minishell *ms)
{
	if (process_heredocs(cmds, ms))
		return (ms->last_status);
	if (exec_pipeline(cmds, ms))
		return (ms->last_status);
	wait_all(ms);
	return (ms->last_status);
}
