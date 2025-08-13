#include "minishell.h"

/* ---------- alloue un nœud ------------------------------------------------ */
t_env	*ft_env_new(char *key, char *val)
{
	t_env	*node;

	node = malloc(sizeof(*node));
	if (!node)
		return (NULL);
	node->key = key;
	node->value = val;
	node->is_exported = true;
	node->next = NULL;
	return (node);
}

/* ---------- ajoute en fin de liste --------------------------------------- */
int	ft_env_add_back(t_env **head, t_env *new)
{
	t_env	*cur;

	if (!new)
		return (-1);
	if (!*head)
		*head = new;
	else
	{
		cur = *head;
		while (cur->next)
			cur = cur->next;
		cur->next = new;
		new->prev = cur;
	}
	return (0);
}

static int	fill_env_array(char **arr, t_env *env, bool all_vars)
{
	int		count;
	char	*kv;

	count = 0;
	while (env)
	{
		if ((all_vars || env->is_exported) && env->value)
		{
			kv = ft_strjoin(env->key, "=");
			if (!kv)
				return (-1);
			arr[count] = ft_strjoin_free(kv, env->value);
			if (!arr[count])
				return (-1);
			count++;
		}
		env = env->next;
	}
	arr[count] = NULL;
	return (0);
}

char	**env_to_array_ext(t_env *env, bool all_vars)
{
	int		count;
	t_env	*cur;
	char	**arr;

	count = 0;
	cur = env;
	while (cur)
	{
		if ((all_vars || cur->is_exported) && cur->value)
			count++;
		cur = cur->next;
	}
	arr = malloc(sizeof(char *) * (count + 1));
	if (!arr)
		return (NULL);
	if (fill_env_array(arr, env, all_vars) == -1)
		return (NULL);
	return (arr);
}

char	**env_to_array(t_env *env)
{
	return (env_to_array_ext(env, false));
}
