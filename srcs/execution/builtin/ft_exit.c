#include "minishell.h"

int	ft_is_numeric(char *str)
{
	int	i;

	i = 0;
	if (!str || !str[0])
		return (0);
	if (str[0] == '-' || str[0] == '+')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (str[i] >= '0' && str[i] <= '9')
			i++;
		else
			return (0);
	}
	return (1);
}

int	builtin_exit(char **args, t_minishell *mini)
{
	int	code;

	if (!args[1])
	{
		printf("exit\n");
		set_exit_status(mini, 0, NULL);
		exit(0);
	}
	if (!ft_is_numeric(args[1]))
	{
		printf("exit\n");
		set_exit_status(mini, 2, "exit: ce n'est pas numérique");
		exit(2);
	}
	if (args[2])
	{
		set_exit_status(mini, 1, "exit: trop d'arguments");
		return (1);
	}
	code = atoi(args[1]) % 256;
	printf("exit\n");
	set_exit_status(mini, code, NULL);
	exit(code);
	return (code);
}
