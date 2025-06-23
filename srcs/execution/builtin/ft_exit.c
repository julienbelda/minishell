#include "minishell.h"

void builtin_exit(char **args, t_minishell *mini)
{
    int	i;

	i = 1;
	int args1 = atoi(args[1]);
	if (!args || !args[0] || !args[1])
		return;
	if (args[0] && !args[1])
	{
		set_exit_status(mini, 0, NULL);
		exit(0);
	}
	while(args[i])
	{
		if (ft_isdigit(args1))	// si c'est un digit apres exit on exit avec le code du digit
		{
			set_exit_status(mini, atoi(args[1]), NULL);
			exit(atoi(args[1]));
			
		}
		else
		{
			printf("exit non valide");
			return;
		}
		if (args[i])
		{
			printf("trop d'args");
			return;
		}
		i++;
	}
}
