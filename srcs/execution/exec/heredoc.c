/* ************************************************************************** */
/*                                heredoc.c                                   */
/* ************************************************************************** */

#include "minishell.h"
#define HD_PROMPT  "> "
extern volatile sig_atomic_t g_signal;

/* Compare la ligne courante au mot-clé EOF */
static int is_eof(char *line, char *eof)
{
    if (!line || !eof)
        return (0);
    return (ft_strcmp(line, eof) == 0);
}

/* Lit le heredoc, renvoie 0 si OK, -1 si Ctrl-C */
int ft_exec_heredoc(char *eof, int *fdread, t_minishell *ms, bool expand)
{
    char    *line;
    int     hd;

    g_signal = 0;
    hd = open(".heredoc.txt", O_CREAT | O_RDWR | O_TRUNC, 0666);
    if (hd == -1)
        return (perror("heredoc"), -1);
    ft_setup_heredoc_signals();
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
    {
        free(line);
        close(hd);
        ft_restore_prompt_signals();
        g_signal = 0;
        unlink(".heredoc.txt");
        return (-1);
    }
    free(line);
    close(hd);
    ft_restore_prompt_signals();
    *fdread = open(".heredoc.txt", O_RDONLY);
    return (0);
}

/* Transforme chaque << en fd prêt à lire */
int process_heredocs(t_command *cmds, t_minishell *ms)
{
    t_redirection   *r;
    int             fd;
    bool            quoted;

    while (cmds)
    {
        r = cmds->redir;
        while (r)
        {
            if (r->type != HEREDOC)
            {
                r = r->next;
                continue;
            }
            quoted = (ft_strchr(r->file, '\'') || ft_strchr(r->file, '"'));
            if (ft_exec_heredoc(r->file, &fd, ms, !quoted) == -1)
            {
                ms->last_status = 130;
                return (1);
            }
            r->type = REDIR_IN;
            r->fd = fd;
            r = r->next;
        }
        cmds = cmds->next;
    }
    return (0);
}

char *ft_expand_line(char *raw, t_minishell *ms)
{
    t_dynbuf    buf;
    size_t      i;
    int         in_squote;

    ft_dynbuf_init(&buf);
    in_squote = 0;
    i = 0;
    while (raw[i])
    {
        if (raw[i] == '\'' && !in_squote)
            in_squote = 1;
        else if (raw[i] == '\'' && in_squote)
            in_squote = 0;
        if (raw[i] == '$' && !in_squote)
        {
            if (ft_expand_variable(&buf, raw, &i, ms) == -1)
                return (ft_dynbuf_free(&buf), NULL);
            continue;
        }
        if (ft_dynbuf_add_char(&buf, raw[i]) == -1)
            return (ft_dynbuf_free(&buf), NULL);
        i++;
    }
    raw = ft_dynbuf_str(&buf);
    ft_dynbuf_free(&buf);
    return (raw);
}
