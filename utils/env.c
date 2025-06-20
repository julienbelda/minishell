#include "minishell.h"

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


void ft_env_add_back(t_env **env, t_env *new)
{
	t_env *last;
	if (!*env)
	{
		*env = new;
		return;
	}
	last = *env;
	while (last->next)
		last = last->next;
	last->next = new;
	new->prev = last;
}

/* static void sort_env_array(t_env **array, int size)
{
	int x = 0;
	while (x < size - 1)
	{
		int y = 0;
		while (y < size - x - 1)
		{
			if (ft_strcmp(array[y]->key, array[y + 1]->key) > 0)
			{
				t_env *tmp = array[y];
				array[y] = array[y + 1];
				array[y + 1] = tmp;
			}
			y++;
		}
		x++;
	}
} */

int print_sorted_env(t_env *env)
{
	t_env *tmp;
	t_env *array[1024];
	int i = 0;

	while (env && i < 1024)
	{
		if (env->is_exported)
			array[i++] = env;
		env = env->next;
	}
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
	return 0;
}