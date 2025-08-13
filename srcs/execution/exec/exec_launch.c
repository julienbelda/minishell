#include "minishell.h"

void	set_std_io(int in_fd, int out_fd)
{
	if (in_fd != STDIN_FILENO && dup2(in_fd, STDIN_FILENO) == -1)
		exit(1);
	if (out_fd != STDOUT_FILENO && dup2(out_fd, STDOUT_FILENO) == -1)
		exit(1);
}

t_env	*dup_env_list(t_env *src)
{
	t_env	*dst;
	t_env	*cur;
	t_env	*node;

	dst = NULL;
	cur = src;
	while (cur)
	{
		node = ft_env_new(ft_strdup(cur->key), ft_strdup(cur->value));
		if (!node)
		{
			ft_free_env_list(dst);
			return (NULL);
		}
		node->is_exported = cur->is_exported;
		ft_env_add_back(&dst, node);
		cur = cur->next;
	}
	return (dst);
}

void	add_assignments_to_env(t_env **env, char **argv)
{
	int		i;
	char	*eq;
	char	*key;
	char	*val;

	i = 0;
	while (argv[i])
	{
		eq = ft_strchr(argv[i], '=');
		if (!eq || eq == argv[i] || ft_strchr(argv[i], '/'))
			break ;
		key = ft_substr(argv[i], 0, eq - argv[i]);
		val = ft_strdup(eq + 1);
		if (key && val)
		{
			set_env(env, key, val);
		}
		free(key);
		free(val);
		i++;
	}
}

// Ajoute une fonction utilitaire pour détecter un préfixe d'assignments
bool	has_assignments_prefix(char **argv)
{
	int	i;
	int	found;

	i = 0;
	found = 0;
	while (argv && argv[i])
		i++;
	i = 0;
	while (argv && argv[i] && is_assignment_str(argv[i]))
	{
		found = 1;
		i++;
	}
	return (found);
}

// Fonction utilitaire pour appliquer les assignments à l'env temporaire
void	add_assignments_prefix_to_env(t_env **env, \
	char **argv, int assign_count)
{
	int		i;
	char	*eq;
	char	*key;
	char	*val;

	i = 0;
	while (i < assign_count && argv[i])
	{
		eq = ft_strchr(argv[i], '=');
		if (!eq || eq == argv[i] || ft_strchr(argv[i], '/'))
			break ;
		key = ft_substr(argv[i], 0, eq - argv[i]);
		val = ft_strdup(eq + 1);
		if (key && val)
		{
			set_env(env, key, val);
		}
		free(key);
		free(val);
		i++;
	}
}
