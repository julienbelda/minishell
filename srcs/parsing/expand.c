#include "minishell.h"

char *ft_env_get(t_env *env, char *key, size_t len)
{
	while (env)
	{
		if (!ft_strncmp(env->key, key, len) && env->key[len] == '\0')
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

/* helper: copie une chaîne entière dans le tampon dynamique */
/* helper: copie une chaîne entière dans le tampon dynamique */
int	append_str_dyn(t_dynbuf *buf, char *src)
{
	char	*current;

	current = src;
	while (*current)
	{
		if (ft_dynbuf_add_char(buf, *current) == -1)
			return (-1);
		current++;
	}
	return (0);
}


int	ft_expand_variable(t_dynbuf *buf, char *line, size_t *idx, t_minishell *mini)
{
	size_t	start;
	char	*value;
	char	*num_str;

	(*idx)++;   /* saute '$' */
	if (line[*idx] == '?')
	{
		num_str = ft_itoa(mini->last_status);
		if (!num_str || append_str_dyn(buf, num_str) == -1)
			return (free(num_str), -1);
		free(num_str);
		(*idx)++;
		return (0);
	}
	start = *idx;
	while (line[*idx]
		&& (ft_isalnum((unsigned char)line[*idx]) || line[*idx] == '_'))
		(*idx)++;
	if (start == *idx)  /* '$' isolé -> littéral */
		return (ft_dynbuf_add_char(buf, '$'));
	value = ft_env_get(mini->env, line + start, *idx - start);
	if (value && append_str_dyn(buf, value) == -1)
		return (-1);
	return (0);
}
