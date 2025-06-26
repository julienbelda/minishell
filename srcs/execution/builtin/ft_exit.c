#include "minishell.h"

int ft_is_numeric(char *str)
{
	int i;

	i = 0;
	if (!str || !str[0])
		return 0;
	if (str[0] == '-' || str[0] == '+')
		i++;
	if (!str[i]) // + ou - seuls => non numérique
		return 0;
	// compris entre 0 et 9 -> valide
	while (str[i])
	{
		if (str[i] >= '0' && str[i] <= '9')
			i++;
		else
			return 0;
	}
	return 1;
}

void builtin_exit(char **args, t_minishell *mini)
{
	if (!args[1])// si il n'existe que exit 
	{
		printf("exit\n");
		set_exit_status(mini, 0, NULL);
		exit(0);
	}
	if (args[1] && !ft_is_numeric(args[1]))
	{
		printf("exit\n");
		set_exit_status(mini, 2, "exit: ce n'est pas numérique");
		exit(2);
	}
	if (args[2])
	{
		set_exit_status(mini, 1, "exit: trop d'arguments");
		return;
	}
	printf("exit\n");
	int code;
	code = (atoi(args[1]) % 256);
	set_exit_status(mini, code, NULL);
	exit(code);
}

/* int main(int argc, char **argv)
{
	if (argc < 2)
		return 0;
	t_minishell mini;
	mini.env = NULL;
	mini.last_status = 0;
	if (ft_strcmp(argv[1], "exit") == 0)
		builtin_exit(argv + 1, &mini);
	// IL FAUT RETURN LE LAST STATUS POUR TROP DARGS SELON BASH
}
 */