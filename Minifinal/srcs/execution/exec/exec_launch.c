#include "minishell.h"

static void	set_std_io(int in_fd, int out_fd)
{
	if (in_fd != STDIN_FILENO && dup2(in_fd, STDIN_FILENO) == -1)
		exit(1);
	if (out_fd != STDOUT_FILENO && dup2(out_fd, STDOUT_FILENO) == -1)
		exit(1);
}

t_env	*dup_env_list(t_env *src)
{
	t_env	*dst;
	t_env	*cur;
	t_env	*node;

	dst = NULL;
	cur = src;
	while (cur)
	{
		node = ft_env_new(ft_strdup(cur->key), ft_strdup(cur->value));
		if (!node)
		{
			ft_free_env_list(dst);
			return (NULL);
		}
		node->is_exported = cur->is_exported;
		ft_env_add_back(&dst, node);
		cur = cur->next;
	}
	return (dst);
}

void	add_assignments_to_env(t_env **env, char **argv)
{
	int		i;
	char	*eq;
	char	*key;
	char	*val;

	i = 0;
	while (argv[i])
	{
		eq = ft_strchr(argv[i], '=');
		if (!eq || eq == argv[i] || ft_strchr(argv[i], '/'))
			break ;
		key = ft_substr(argv[i], 0, eq - argv[i]);
		val = ft_strdup(eq + 1);
		if (key && val)
		{
			set_env(env, key, val);
		}
		free(key);
		free(val);
		i++;
	}
}

// Ajoute une fonction utilitaire pour détecter un préfixe d'assignments
bool	has_assignments_prefix(char **argv)
{
	int	i;
	int	found;

	i = 0;
	found = 0;
	while (argv && argv[i])
		i++;
	i = 0;
	while (argv && argv[i] && is_assignment_str(argv[i]))
	{
		found = 1;
		i++;
	}
	return (found);
}

// Fonction utilitaire pour appliquer les assignments à l'env temporaire
void	add_assignments_prefix_to_env(t_env **env, \
	char **argv, int assign_count)
{
	int		i;
	char	*eq;
	char	*key;
	char	*val;

	i = 0;
	while (i < assign_count && argv[i])
	{
		eq = ft_strchr(argv[i], '=');
		if (!eq || eq == argv[i] || ft_strchr(argv[i], '/'))
			break ;
		key = ft_substr(argv[i], 0, eq - argv[i]);
		val = ft_strdup(eq + 1);
		if (key && val)
		{
			set_env(env, key, val);
		}
		free(key);
		free(val);
		i++;
	}
}

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
	// Fermer les pipes inutiles dans l'enfant (mais pas ceux dupliqués vers STDIN/STDOUT)
	// Note: après dup2, STDIN/STDOUT pointent vers les pipes, donc on ne les ferme pas
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
