#include "minishell.h"

char *get_env(t_env *env, const char *key)
{
	while(env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

int set_env(t_env **env, char *key, const char *value)
{
	// je peux modifier une variable d'env si elle existe 
	t_env *tmp;
	t_env *new_nodes;
	tmp = *env;

	while (tmp)
	{
		if (ft_strcmp(tmp->key, key) == 0)
		{
			free(tmp->value);
			tmp->value = ft_strdup(value);
			return 0;
		}
		tmp = tmp->next;
	}
	new_nodes = create_env_with_kv(key, value);
	if (!new_nodes)
		return 1;
	new_nodes->next = *env;
	if (*env)
    	(*env)->prev = new_nodes;   /* ← AJOUT   */
	new_nodes->prev = NULL;         /* tête */
	*env = new_nodes;
	return 0;
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


void ft_env_add_back2(t_env **env, t_env *new)
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

void sort_env_array(t_env **array, int size)
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
}

int print_sorted_env(t_env *env)
{
	/* t_env *tmp; */
	t_env *array[1024];
	int i = 0;

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
	return 0;
}

/* Tu dois avoir une fonction qui peut :

chercher une variable dans t_env *mini->env

renvoyer sa valeur (comme get_env_value(mini->env, "HOME")) */