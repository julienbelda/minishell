#include "minishell.h"

int	parse_pipe(t_command **cur, int *err)
{
	if (!(*cur)->name)
		return (*err = 1, -1);
	(*cur)->next = ft_init_cmd();
	if (!(*cur)->next)
		return (*err = -1, -1);
	*cur = (*cur)->next;
	return (0);
}

int	parse_redir(t_command *cmd, t_token **tok, int *err)
{
	t_token_type	type;

	type = (*tok)->type;
	*tok = (*tok)->next;
	if (!(*tok) || (*tok)->type != WORD)
		return (*err = 1, -1);
	if (redir_push(cmd, type, (*tok)->value) == -1)
		return (*err = -1, -1);
	return (0);
}

int	handle_word_token(t_command *cur, t_token **tok)
{
	if (parse_word(cur, *tok) == -1)
		return (-1);
	*tok = (*tok)->next;
	return (0);
}
