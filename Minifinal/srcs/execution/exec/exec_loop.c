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

static int	exec_pipeline_cmd(t_command *cmd, \
	t_minishell *ms, int *in_fd, int pipefd[2])
{
	int	out_fd;
	pid_t pid;

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
	// Parent: ferme l'extrémité d'écriture du pipe immédiatement après le fork
	if (cmd->next)
		close(pipefd[1]);
	// Parent: ferme l'extrémité de lecture précédente (sauf STDIN), sauf pour la dernière commande
	if (cmd->next && *in_fd != STDIN_FILENO)
		close(*in_fd);
	// Prépare in_fd pour la prochaine commande
	if (cmd->next)
		*in_fd = pipefd[0];
	else
		*in_fd = STDIN_FILENO;
	return (0);
}

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
	// Ferme le dernier pipe de lecture si besoin
	if (in_fd != STDIN_FILENO)
		close(in_fd);
	return (0);
}

static void	wait_all(t_minishell *ms)
{
	int	status;
	int	pid;

	while ((pid = waitpid(-1, &status, 0)) > 0)
	{
		if (WIFEXITED(status))
			ms->last_status = WEXITSTATUS(status);
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
