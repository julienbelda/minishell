#include "minishell.h"

int update_var_oldpwd_pwd(t_env **env)
{
	char cwd[1024];
	char *oldpwd;

	if (!getcwd(cwd, sizeof(cwd)))
		return 1;
	oldpwd = get_env(*env, "PWD");
	if (!oldpwd)
		return 1;
	if (set_env(*env, "OLDPWD", oldpwd) != 0)
		return 1;
	if (set_env(*env, "PWD", cwd) != 0)
		return 1;
	return 0;
}

int handle_home_cd(t_env **env)
{
	char *path_home;

	path_home = get_env(*env, "HOME");
	if (!path_home)
	{
		ft_putstr_fd("le path_home n'existe pas\n", 2);
		return 1;
	}
	if (chdir(path_home) != 0)
	{
		perror("cd");
		return 1;
	}
	return (update_var_oldpwd_pwd(env));
}

int handle_oldpwd(t_env **env)
{
	char *oldpwd;

	oldpwd = get_env(*env, "OLDPWD");
	if(!oldpwd)
	{
		ft_putstr_fd("erreur: oldpwd n'existe pas\n", 2);
		return 1;
	}
	if (chdir(oldpwd) != 0)
	{
		perror("cd");
		return 1;
	}
	printf("%s\n", oldpwd);
	return (update_var_oldpwd_pwd(env));
}

void builtin_cd(char **args, t_env **env, t_minishell *mini)
{
	if (!args[1] || (ft_strcmp(args[1], "~") == 0))
		return handle_home_cd(env);
	if (args[1] && (ft_strcmp(args[1], "-") == 0))
		return handle_oldpwd(env);
	if (chdir(args[1]) != 0)
	{
		perror("cd");
		return 1;
	}
	return update_var_oldpwd_pwd(env); // -> ERREUR j'actualise oldpwd apres le chdir A MODIFIER !
										// + ajouter last status 
}