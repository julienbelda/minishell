#include "minishell.h"


int	ft_lex_operator(t_dynbuf *b, t_token **lst,
			char *l, size_t *i)
{
	if (!ft_is_operator(l[*i]))
		return (0);
	if (ft_flush_word(b, lst) == -1
		|| !ft_found_operator_token(lst, l, i))
		return (-1);
	return (1);
}

int	ft_lex_quote_toggle(t_lstate *s, char *l, size_t *i)
{
	if (*s == NORMAL && (l[*i] == '"' || l[*i] == '\''))
	{
		if (l[*i] == '\'')
			*s = IN_SQUOTE;
		else
			*s = IN_DQUOTE;
		(*i)++;
		return (1);
	}
	if ((*s == IN_SQUOTE && l[*i] == '\'')
		|| (*s == IN_DQUOTE && l[*i] == '"'))
	{
		(*i)++;
		*s = NORMAL;
		return (1);
	}
	return (0);
}

int	ft_lex_variable(t_dynbuf *buf, char *line, size_t *i,
			t_minishell *ms)
{
	if (line[*i] != '$')
		return (0);
	if (ft_expand_variable(buf, line, i, ms) == -1)
		return (-1);
	return (1);
}

int	ft_lex_space(t_dynbuf *b, t_token **lst,
			char *l, size_t *i)
{
	if (!isspace((unsigned char)l[*i]))
		return (0);
	if (ft_flush_word(b, lst) == -1)
		return (-1);
	(*i)++;
	return (1);
}

int  ft_lex_loop(t_lexctx *c, char *line)
{
    size_t  i;
    int     r;

    i = 0;
    while (line[i])
    {
        r = 0;
        if (c->st == NORMAL)
            r = ft_lex_operator(&c->buf, &c->lst, line, &i);
        if (r == 0)
            r = ft_lex_quote_toggle(&c->st, line, &i);
        if (r == 0 && c->st != IN_SQUOTE)
            r = ft_lex_variable(&c->buf, line, &i, c->ms);
        if (r == 0 && c->st == NORMAL)
            r = ft_lex_space(&c->buf, &c->lst, line, &i);
        if (r == -1)
            return (-1);
        if (r == 1)
            continue ;
        if (ft_dynbuf_add_char(&c->buf, line[i++]) == -1)
            return (-1);
    }
    if (c->st != NORMAL || ft_flush_word(&c->buf, &c->lst) == -1)
        return (-1);
    return (0);
}

t_token *ft_lexer(char *line, t_minishell *ms)
{
    t_lexctx    ctx;

    ft_dynbuf_init(&ctx.buf);
    ctx.lst = NULL;
    ctx.st = NORMAL;
    ctx.ms = ms;
    if (ft_lex_loop(&ctx, line) == -1)
        return (ft_lex_error(&ctx.buf, &ctx.lst, "lexer error"));
    ft_dynbuf_free(&ctx.buf);
    return (ctx.lst);
}