#include "minishell.h"

static int	process_command_heredocs(t_command *cmd, t_minishell *ms)
{
	t_redirection	*r;

	r = cmd->redir;
	while (r)
	{
		if (process_single_heredoc(r, ms) == 1)
			return (1);
		r = r->next;
	}
	return (0);
}

int	process_heredocs(t_command *cmds, t_minishell *ms)
{
	while (cmds)
	{
		if (process_command_heredocs(cmds, ms) == 1)
			return (1);
		cmds = cmds->next;
	}
	return (0);
}

static int	expand_line_loop(char *raw, t_dynbuf *buf, t_minishell *ms)
{
	size_t	i;
	int		in_squote;

	i = 0;
	in_squote = 0;
	while (raw[i])
	{
		if (raw[i] == '\'' && !in_squote)
			in_squote = 1;
		else if (raw[i] == '\'' && in_squote)
			in_squote = 0;
		if (raw[i] == '$' && !in_squote)
		{
			if (ft_expand_variable(buf, raw, &i, ms) == -1)
				return (-1);
			continue ;
		}
		if (ft_dynbuf_add_char(buf, raw[i]) == -1)
			return (-1);
		i++;
	}
	return (0);
}

char	*ft_expand_line(char *raw, t_minishell *ms)
{
	t_dynbuf	buf;
	char		*result;

	ft_dynbuf_init(&buf);
	if (expand_line_loop(raw, &buf, ms) == -1)
		return (ft_dynbuf_free(&buf), NULL);
	result = ft_dynbuf_str(&buf);
	ft_dynbuf_free(&buf);
	return (result);
}
