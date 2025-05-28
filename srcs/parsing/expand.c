#include "minishell.h"

static char *ft_env_get(t_env *env, char *key, size_t len)
{
	while (env)
	{
		if (!ft_strncmp(env->key, key, len) && env->key[len] == '\0')
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

static int	ft_expand_variable(t_dynbuf *b, char *l, size_t *i, t_minishell *ms)
{
	size_t	s;
	size_t	j;
	char	*val;
	char	*temp;

	(*i)++;
	if (l[*i] == '?')
	{
		j = 0;
		if (!(temp = ft_itoa(ms->last_status)))
			return (-1);
		while (temp[j])
			if (ft_dynbuf_add_char(b, temp[j++]) == -1)
				return(free(temp), 1);
		free(temp);
		(*i++);
		return (0);
	}
	s = *i;
	while (ft_isalnum(l[*i]) || l[*i] == '_')
		(*i)++;
	if (s == *i)
		return (ft_dynbuf_add_char(b, '$'));
	val = ft_env_get(ms->env, l + s, *i - s);
	if (!val)
		return (0);
	while(*val)
		if (ft_dynbuf_add_char(b, *val++) == -1)
			return (-1);
	return (0);
}