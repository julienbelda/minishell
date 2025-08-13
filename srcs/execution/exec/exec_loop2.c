#include "minishell.h"

static int	exec_pipeline(t_command *cmds, t_minishell *ms)
{
	int	pipefd[2];
	int	in_fd;

	in_fd = STDIN_FILENO;
	while (cmds)
	{
		if (exec_pipeline_cmd(cmds, ms, &in_fd, pipefd))
			return (1);
		cmds = cmds->next;
	}
	if (in_fd != STDIN_FILENO)
		close(in_fd);
	return (0);
}

static void	wait_all(t_minishell *ms)
{
	int	status;
	int	pid;

	pid = waitpid(-1, &status, 0);
	while (pid > 0)
	{
		if (WIFEXITED(status))
			ms->last_status = WEXITSTATUS(status);
		pid = waitpid(-1, &status, 0);
	}
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
