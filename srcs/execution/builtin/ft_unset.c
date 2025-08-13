#include "minishell.h"

int	builtin_unset(char **args, t_minishell *mini)
{
	int		i;
	t_env	*temp;

	i = 0;
	while (args[i])
	{
		temp = mini->env;
		while (temp)
		{
			if (ft_strcmp(temp->key, args[i]) == 0)
			{
				remove_node_target(&mini->env, temp);
				break ;
			}
			temp = temp->next;
		}
		i++;
	}
	set_exit_status(mini, 0, NULL);
	return (0);
}
