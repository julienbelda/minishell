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

char **env_to_array(t_env *env)
{
    int   count = 0;
    t_env *cur  = env;

    /* 1) compte */
    while (cur)
    {
        if (cur->is_exported && cur->value)
            count++;
        cur = cur->next;
    }
    char **arr = malloc(sizeof(char *) * (count + 1));
    if (!arr)
        return NULL;

    /* 2) copie "key=value" */
    cur   = env;
    count = 0;
    while (cur)
    {
        if (cur->is_exported && cur->value)
        {
            char *kv = ft_strjoin(cur->key, "=");
            if (!kv)
                return arr[count] = NULL, arr; /* laisse execve gérer */
            arr[count] = ft_strjoin_free(kv, cur->value); /* helper qui free kv */
            if (!arr[count])
                return arr[count] = NULL, arr;
            count++;
        }
        cur = cur->next;
    }
    arr[count] = NULL;
    return arr;
}

void free_env_array(char **arr)
{
    if (!arr)
        return;
    for (int i = 0; arr[i]; i++)
        free(arr[i]);
    free(arr);
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