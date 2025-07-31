#include "minishell.h"

int	ft_pwd(char **argv, t_minishell *ms)
{
	char	cwd[PATH_MAX];

	(void)argv;
	if (getcwd(cwd, sizeof(cwd)))
	{
		printf("%s\n", cwd);
		set_exit_status(ms, 0, NULL);
		return (0);
	}
	else
	{
		perror("pwd");
		set_exit_status(ms, 1, "pwd: error");
		return (1);
	}
}
