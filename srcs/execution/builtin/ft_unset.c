#include "minishell.h"

int builtin_unset(char **args, t_minishell *mini)
{
    // retirer une var en tapant son key : ex -> unset PATH supprime PATH=
    int i;

	i = 0;
	t_env *temp;
	// je dois comparer chaque args avec chaque key de l'env
	while (args[i])
	{
		temp = mini->env;
		while(temp)
		{
			if (ft_strcmp(temp->key, args[i]) == 0)
			{	
				remove_node_target(&mini->env, temp);
				break;
			}
			temp = temp->next;
		}
		i++;
	}
	set_exit_status(mini, 0, NULL);
	return 0;
}
