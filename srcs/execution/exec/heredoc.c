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

int	process_single_heredoc(t_redirection *r, t_minishell *ms)
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
