#include "minishell.h"

bool	ft_is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

int	ft_flush_word(t_dynbuf *b, t_token **list)
{
	char	*str;

	str = ft_dynbuf_str(b);
	if (!str)
		return (0);
	if (!ft_add_token(list, WORD, str))
		return (-1);
	b->len = 0;
	if (b->data)
		b->data[0] = '\0';
	return (0);
}

t_token	*ft_lex_error(t_dynbuf *b, t_token **lst, const char *msg)
{
	ft_dynbuf_free(b);
	ft_token_clear(lst);
	printf("LEXER-ERR: %s\n", msg);
	return (NULL);
}
