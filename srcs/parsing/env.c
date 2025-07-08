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
	}
	return (0);
}

/* ---------- sépare "KEY=VAL" --------------------------------------------- */
int	ft_split_key_value(char *src, char **key, char **val)
{
	size_t	i;

	i = 0;
	while (src[i] && src[i] != '=')
		i++;
	*key = ft_substr(src, 0, i);
	if (!*key)
		return (-1);
	if (src[i] == '=')
		*val = ft_strdup(src + i + 1);
	else
		*val = ft_strdup("");
	if (!*val)
		return (free(*key), -1);
	return (0);
}

/* ---------- crée la liste complète --------------------------------------- */
t_env	*init_env(char **envp)
{
	t_env	*head;
	t_env	*node;
	char	*key;
	char	*val;

	head = NULL;
	while (*envp)
	{
		if (ft_split_key_value(*envp, &key, &val) == -1)
			return (ft_free_env_list(head), NULL);
		node = ft_env_new(key, val);
		if (!node || ft_env_add_back(&head, node) == -1)
			return (ft_free_env_list(head), NULL);
		envp++;
	}
	return (head);
}

void	ft_free_env_list(t_env *head)
{
	t_env	*tmp;

	while (head)
	{
		tmp = head->next;
		free(head->key);
		free(head->value);
		free(head);
		head = tmp;
	}
}