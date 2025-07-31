/* ************************************************************************** */
/*                                heredoc.c                                   */
/* ************************************************************************** */
#include "minishell.h"
#define HD_PROMPT  "> "

extern volatile sig_atomic_t	g_signal;

/* Compare la ligne courante au mot-clé EOF */
static int	is_eof(char *line, char *eof)
{
	if (!line || !eof)
		return (0);
	return (ft_strcmp(line, eof) == 0);
}

static int	handle_heredoc_error(char *line, int hd)
{
	free(line);
	close(hd);
	ft_restore_prompt_signals();
	g_signal = 0;
	unlink(".heredoc.txt");
	return (-1);
}

static int	read_heredoc_loop(char *eof, int hd, t_minishell *ms, bool expand)
{
	char	*line;

	line = readline("> ");
	while (line && !is_eof(line, eof) && g_signal != SIGINT)
	{
		if (expand)
			line = ft_expand_line(line, ms);
		write(hd, line, ft_strlen(line));
		write(hd, "\n", 1);
		if (line && *line)
			add_history(line);
		free(line);
		line = readline("> ");
	}
	if (g_signal == SIGINT)
		return (handle_heredoc_error(line, hd));
	free(line);
	return (0);
}

int	ft_exec_heredoc(char *eof, int *fdread, t_minishell *ms, bool expand)
{
	int		hd;

		g_signal = 0;
	hd = open(".heredoc.txt", O_CREAT | O_RDWR | O_TRUNC, 0666);
	if (hd == -1)
		return (perror("heredoc"), -1);
	ft_setup_heredoc_signals();
	if (read_heredoc_loop(eof, hd, ms, expand) == -1)
		return (-1);
	close(hd);
	ft_restore_prompt_signals();
	*fdread = open(".heredoc.txt", O_RDONLY);
	return (0);
}

static int	process_single_heredoc(t_redirection *r, t_minishell *ms)
{
	int		fd;
	bool	quoted;

			if (r->type != HEREDOC)
		return (0);
			quoted = (ft_strchr(r->file, '\'') || ft_strchr(r->file, '"'));
			if (ft_exec_heredoc(r->file, &fd, ms, !quoted) == -1)
			{
				ms->last_status = 130;
				return (1);
			}
			r->type = REDIR_IN;
			r->fd = fd;
	return (0);
}

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
