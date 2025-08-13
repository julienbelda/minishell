#include "minishell.h"

static char	**get_external_env(t_command *cmd, t_minishell *ms)
{
	t_env	*tmp_env;
	char	**envp;

	if (cmd->assign_count > 0)
	{
		tmp_env = dup_env_list(ms->env);
		if (!tmp_env)
			return (NULL);
		add_assignments_prefix_to_env(&tmp_env, \
			cmd->argv_full, cmd->assign_count);
		envp = env_to_array_ext(tmp_env, true);
		ft_free_env_list(tmp_env);
		return (envp);
	}
	return (env_to_array(ms->env));
}

static void	exec_external(t_command *cmd, t_minishell *ms)
{
	char	*path;
	char	**envp;

	path = get_cmd_path(cmd->name, ms->env);
	if (!path)
	{
		fprintf(stderr, "%s: command not found\n", cmd->name);
		exit(127);
	}
	envp = get_external_env(cmd, ms);
	if (!envp)
		exit(1);
	execve(path, cmd->argv, envp);
	perror(cmd->name);
	free_envp(envp);
	if (errno == ENOENT)
		exit(127);
	else
		exit(126);
}

static void	child_process(t_command *cmd, int in_fd, int out_fd,
	t_minishell *ms)
{
	int		ret;
	t_env	*tmp_env;

	ft_setup_child_signals();
	set_std_io(in_fd, out_fd);
	if (apply_redirections(cmd->redir) == -1)
		exit(1);
	if (is_builtin(cmd))
	{
		tmp_env = NULL;
		if (cmd->assign_count > 0)
		{
			tmp_env = dup_env_list(ms->env);
			if (!tmp_env)
				exit(1);
			add_assignments_prefix_to_env(&tmp_env, \
				cmd->argv, cmd->assign_count);
			ret = run_builtin_env(cmd, ms, tmp_env);
			ft_free_env_list(tmp_env);
			exit(ret);
		}
		exit(run_builtin(cmd, ms));
	}
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
