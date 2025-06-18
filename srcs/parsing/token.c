#include "minishell.h"

/* 	alloue un t_token, le chaîne à la fin de la liste double, 
	stocke type et value (qui peut être NULL pour les opérateurs). */

int	ft_add_token(t_token **list, t_token_type type, char *value)
{
	t_token	*new;
	t_token	*tail;

	new = malloc(sizeof(*new));
	if (!new)
	{
		free(value); //Value peut être alloué par ft_dyn_buff_str.
		return (free(value), false);
	}
	new->type = type;
	new->value = value;
	new->next = NULL;
	new->prev = NULL;
	if (!*list)
		*list = new;
	else
	{
		tail = *list;
		while (tail->next)
			tail = tail->next;
		tail->next = new;
		new->prev = tail;
	}
	return (true);
}

int	ft_found_operator_token(t_token **list, char *line, size_t *i)
{
	if (line[*i] == '|')
	{
		(*i)++;
		return (ft_add_token(list, PIPE, NULL));
	}
	else if (line[*i] == '<')
	{
		if (line[*i + 1] && line[*i + 1] == '<')
		{
			*i += 2;
			return (ft_add_token(list, HEREDOC, NULL));
		}
		(*i)++;
		return (ft_add_token(list, REDIR_IN, NULL));
	}
	else
	{
		if (line[*i + 1] && line[*i + 1] == '>')
		{
			*i += 2;
			return (ft_add_token(list, REDIR_APPEND, NULL));
		}
		(*i)++;
		return (ft_add_token(list, REDIR_OUT, NULL));
	}
}

/* libère value + le nœud pour toute la liste.*/

void	ft_token_clear(t_token **lst)
{
	t_token	*cur;
	t_token	*tmp;

	if (!lst || !*lst)
		return ;
	cur = *lst;
	while (cur)
	{
		tmp = cur->next;
		free(cur->value);
		free(cur);
		cur = tmp;
	}
	*lst = NULL;
}