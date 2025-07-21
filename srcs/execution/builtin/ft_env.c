#include "minishell.h"

int	ft_env(char **argv, t_minishell *ms)
{
	t_env	*cur;

	(void)argv;
	cur = ms->env;
	while (cur)
	{
		if (cur->is_exported && cur->value)
			printf("%s=%s\n", cur->key, cur->value);
		cur = cur->next;
	}
	return (0);
}
