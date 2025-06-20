#include "minishell.h"

void count_args(char **args)
{
	int	count;

	count = 0;
	while(args[count])
		count++;
	return (count);
}

void update_oldpwd(t_command *cmds, t_minishell *mini)
{
	
}