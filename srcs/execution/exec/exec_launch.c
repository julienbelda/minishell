#include "minishell.h"

static void	set_std_io(int in_fd, int out_fd)
{
	if (in_fd != STDIN_FILENO && dup2(in_fd, STDIN_FILENO) == -1)
		exit(1);
	if (out_fd != STDOUT_FILENO && dup2(out_fd, STDOUT_FILENO) == -1)
		exit(1);
}

static void	exec_external(t_command *cmd, t_minishell *ms)
{
	char	*path;
	char	**envp;

	path = get_cmd_path(cmd->name, ms->env);
	if (!path)
		fprintf (stderr, "%s: command not found\n", cmd->name), exit(127);
	envp = env_to_array(ms->env);
	if (!envp)
		exit(1);
	execve(path, cmd->argv, envp);
	perror(cmd->name);
	free_envp(envp);
	exit (errno == ENOENT ? 127 : 126);
}

static void	child_process(t_command *cmd, int in_fd, int out_fd,
	t_minishell *ms)
{
	ft_setup_child_signals();
	set_std_io(in_fd, out_fd);
	if (apply_redirections(cmd->redir) == -1)
		exit(1);
	if (is_builtin(cmd))
		exit(run_builtin(cmd, ms));
	exec_external(cmd, ms);
}

pid_t	launch_child(t_command *cmd, int in_fd, int out_fd, t_minishell *ms)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		return (-1);
	if (pid == 0)
		child_process(cmd, in_fd, out_fd, ms);
	return (pid);
}
