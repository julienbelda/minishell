#include "minishell.h"

int	update_var_oldpwd_pwd(t_env **env)
{
	char	cwd[1024];
	char	*oldpwd;

	if (!getcwd(cwd, sizeof(cwd)))
		return (1);
	oldpwd = get_env(*env, "PWD");
	if (!oldpwd)
		return (1);
	if (set_env(env, "OLDPWD", oldpwd) != 0)
		return (1);
	if (set_env(env, "PWD", cwd) != 0)
		return (1);
	return (0);
}

int	handle_home_cd(t_env **env)
{
	char	*path_home;

	path_home = get_env(*env, "HOME");
	if (!path_home)
	{
		ft_putstr_fd("le path_home n'existe pas\n", 2);
		return (1);
	}
	if (chdir(path_home) != 0)
	{
		perror("cd");
		return (1);
	}
	return (update_var_oldpwd_pwd(env));
}

int	handle_oldpwd(t_env **env)
{
	char	*oldpwd;

	oldpwd = get_env(*env, "OLDPWD");
	if (!oldpwd)
	{
		ft_putstr_fd("erreur: oldpwd n'existe pas\n", 2);
		return (1);
	}
	if (chdir(oldpwd) != 0)
	{
		perror("cd");
		return (1);
	}
	printf("%s\n", oldpwd);
	return (update_var_oldpwd_pwd(env));
}

int	ft_cd(char **argv, t_minishell *ms)
{
	t_env	**env;
	int		ret;

	env = &ms->env;
	if (!argv[1] || !ft_strcmp(argv[1], "~"))
		ret = handle_home_cd(env);
	else if (!ft_strcmp(argv[1], "-"))
		ret = handle_oldpwd(env);
	else if (chdir(argv[1]) != 0)
	{
		perror("cd");
		ret = 1;
	}
	else
		ret = update_var_oldpwd_pwd(env);
	ms->last_status = ret;
	return (ret);
}
