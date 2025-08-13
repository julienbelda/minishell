#include "minishell.h"

char	*get_env(t_env *env, const char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

int	set_env(t_env **env, char *key, const char *value)
{
	t_env	*tmp;
	t_env	*new_nodes;

	tmp = *env;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, key) == 0)
		{
			free(tmp->value);
			tmp->value = ft_strdup(value);
			if (tmp->value)
				return (0);
			return (1);
		}
		tmp = tmp->next;
	}
	new_nodes = create_env_with_kv(key, value);
	if (!new_nodes)
		return (1);
	new_nodes->next = *env;
	if (*env)
		(*env)->prev = new_nodes;
	new_nodes->prev = NULL;
	*env = new_nodes;
	return (0);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s2[i])
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

void	sort_env_array(t_env **array, int size)
{
	int		x;
	int		y;
	t_env	*tmp;

	x = 0;
	while (x < size - 1)
	{
		y = 0;
		while (y < size - x - 1)
		{
			if (ft_strcmp(array[y]->key, array[y + 1]->key) > 0)
			{
				tmp = array[y];
				array[y] = array[y + 1];
				array[y + 1] = tmp;
			}
			y++;
		}
		x++;
	}
}

int	print_sorted_env(t_env *env)
{
	t_env	*array[1024];
	int		i;

	i = 0;
	while (env && i < 1024)
	{
		if (env->is_exported)
			array[i++] = env;
		env = env->next;
	}
	array[i] = NULL;
	sort_env_array(array, i);
	i = 0;
	while (i < 1024 && array[i])
	{
		if (array[i]->value)
			printf("declare -x %s=\"%s\"\n", array[i]->key, array[i]->value);
		else
			printf("declare -x %s\n", array[i]->key);
		i++;
	}
	return (0);
}
