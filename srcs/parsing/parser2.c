#include "minishell.h"

int	handle_pipe_token(t_command **cur, t_token **tok, int *err)
{
	if (parse_pipe(cur, err) == -1)
		return (-1);
	*tok = (*tok)->next;
	return (0);
}

int	handle_redir_token(t_command *cur, t_token **tok, int *err)
{
	if (parse_redir(cur, tok, err) == -1)
		return (-1);
	*tok = (*tok)->next;
	return (0);
}

int	process_token(t_command **cur, t_token **tok, int *err)
{
	if ((*tok)->type == WORD)
		return (handle_word_token(*cur, tok));
	else if ((*tok)->type == PIPE)
		return (handle_pipe_token(cur, tok, err));
	else
		return (handle_redir_token(*cur, tok, err));
}

t_command	*parse_tokens(t_token *tok, int *err)
{
	t_command	*head;
	t_command	*cur;

	*err = 0;
	head = ft_init_cmd();
	if (!head)
		return (NULL);
	cur = head;
	while (tok)
	{
		if (process_token(&cur, &tok, err) == -1)
			return (NULL);
	}
	if (!cur->name)
		return (head);
	return (head);
}

int	redir_push(t_command *cmd, t_token_type type, char *file)
{
	t_redirection	*node;
	t_redirection	*cur;

	node = malloc(sizeof(*node));
	if (!node)
		return (-1);
	node->type = type;
	node->file = file;
	node->next = NULL;
	if (!cmd->redir)
		cmd->redir = node;
	else
	{
		cur = cmd->redir;
		while (cur->next)
			cur = cur->next;
		cur->next = node;
	}
	return (0);
}
