/* ************************************************************************** */
/*                                heredoc.c                                   */
/* ************************************************************************** */
#include "minishell.h"
#define HD_PROMPT  "> "

/* -------- helpers : nom de fichier tmp ----------------------------------- */
static char *hd_tmpname(void)
{
    char    *name;
    int     fd;
    int     i;

    i = 0;
    while (1)
    {
        name = ft_strjoin("/tmp/minishell_hd_", ft_itoa(i));
        if (!name)
            return (NULL);
        fd = open(name, O_CREAT | O_EXCL | O_RDWR, 0600);
        if (fd >= 0)
            return (close(fd), name);
        free(name);
        ++i;
    }
}

/* -------- écrit le contenu et renvoie chemin ----------------------------- */
static char *hd_fill_file(const char *delim, bool expand, t_minishell *ms)
{
    char    *tmp = hd_tmpname();
    int     fd;
    char    *line;

    if (!tmp)
        return (NULL);
    fd = open(tmp, O_WRONLY | O_TRUNC, 0600);
    if (fd == -1)
        return (free(tmp), NULL);
    while (1)
    {
        line = readline(HD_PROMPT);
        if (!line || ft_strcmp(line, delim) == 0)
            break ;
        if (expand)
            line = ft_expand_line(line, ms);     /* ← utilitaire d’expansion   */
        write(fd, line, ft_strlen(line));
        write(fd, "\n", 1);
        free(line);
    }
    free(line);
    close(fd);
    return (tmp);
}

char    *ft_expand_line(char *raw, t_minishell *ms)
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
            continue ;
        }
        if (ft_dynbuf_add_char(&buf, raw[i]) == -1)
            return (ft_dynbuf_free(&buf), NULL);
        i++;
    }
    raw = ft_dynbuf_str(&buf);     /* duplique le contenu terminé      */
    ft_dynbuf_free(&buf);          /* libère le tampon interne         */
    return (raw);                  /* peut être NULL si malloc échoue  */
}

/* -------- API : traiter tous les heredocs de la ligne -------------------- */
int process_heredocs(t_command *cmds, t_minishell *ms)
{
    t_redirection   *r;
    char            *file;
    bool            quoted;

    while (cmds)
    {
        r = cmds->redir;
        while (r)
        {
            if (r->type == HEREDOC)
            {
                quoted = (ft_strchr(r->file, '\'') || ft_strchr(r->file, '\"'));
                file = hd_fill_file(r->file, !quoted, ms);
                if (!file)
                    return (perror("heredoc"), 1);
                r->type = REDIR_IN;
                r->file = file;
            }
            r = r->next;
        }
        cmds = cmds->next;
    }
    return (0);
}
