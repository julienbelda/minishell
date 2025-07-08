#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include "../srcs/libft/libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <string.h>
# include <signal.h>
# include <unistd.h>
# include <errno.h>
# include <limits.h>
# include <termios.h>
# include <fcntl.h>
# include <stddef.h>
# include <stdbool.h>
# include <ctype.h>
# include <sys/wait.h>
# include <sys/param.h>

typedef enum e_type
{
	WORD,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	HEREDOC
}	t_token_type;

typedef enum e_lstate
{
    NORMAL,
    IN_SQUOTE,
    IN_DQUOTE
}   t_lstate;

typedef struct s_token
{
	t_token_type	type; //WORD, PIPE..
	char			*value; // Mot, nom d'un fichier, délimiteur..
	struct s_token	*next;
	struct s_token	*prev;

}	t_token;

typedef struct s_redirection
{
	t_token_type			type; //REDIR_IN, REDIR_OUT, REDIR_APPEND, HEREDOC..
	char					*file; //File ou délim si heredoc
	struct s_redirection	*next;
}	t_redirection;

typedef struct s_command
{
	char            *name;
	char            **argv;
	size_t          argv_cap;   /* capacité actuelle du tableau */
	size_t          argc;       /* nb d’arguments remplis       */
	t_redirection   *redir;
	struct s_command *next;
}	t_command;

typedef struct s_env
{
	char			*key; //Ce qui y'a avant "="
	char			*value; //Valeur de la variable apres "="
	bool			is_exported; //1 visible dans l'env
	struct s_env	*next;
	struct s_env	*prev;
}	t_env;

typedef struct s_minishell
{
	t_env	*env;
	int		last_status; //Pour $?
}	t_minishell;

typedef struct s_dynbuf // Structure utilisé que dans lexer pour construire pas a pas un mot de longeur inconnue avant de le placer dans token
{
	char	*data; // Zone malloc-ée
	size_t	len;	// Nb de caractère utilisé
	size_t	capacity; // capacité allouée
}	t_dynbuf;

typedef struct s_lexctx
{
    t_dynbuf    buf;
    t_token     *lst;
    t_lstate    st;
    t_minishell *ms;
}   t_lexctx;

t_token		*ft_lexer(char *line, t_minishell *mini);
t_command	*ft_parser(t_token *tokens);
int			ft_execute(t_command *commands, t_minishell *mini);

int			ft_echo(char **args, t_minishell *mini);
int			ft_cd(char **args, t_minishell *mini);
int			ft_pwd(char **args, t_minishell *mini);
int			ft_export(char **args, t_minishell *mini);
int			ft_unset(char **args, t_minishell *mini);
int			ft_env(char **args, t_minishell *mini);
int			ft_exit(char **args, t_minishell *mini);

/* DYNBUF.C */

void		ft_dynbuf_init(t_dynbuf *b);
int			ft_buf_grow(t_dynbuf *b, size_t need);
int			ft_dynbuf_add_char(t_dynbuf *b, char c);
char		*ft_dynbuf_str(t_dynbuf *b);
void		ft_dynbuf_free(t_dynbuf *b);

/* EXPAND.C */

char		*ft_env_get(t_env *env, char *key, size_t len);
int			append_str_dyn(t_dynbuf *buf, char *src);
int			ft_expand_variable(t_dynbuf *buf, char *line, size_t *idx, t_minishell *mini);

/* LEXER_HELPER.C */

bool		ft_is_operator(char c);
int			ft_flush_word(t_dynbuf *b, t_token **list);
t_token		*ft_lex_error(t_dynbuf *b, t_token **lst, const char *msg);

/* LEXER.C */
int			ft_lex_operator(t_dynbuf *b, t_token **lst, char *l, size_t *i);
int			ft_lex_quote_toggle(t_lstate *s, char *l, size_t *i);
int			ft_lex_variable(t_dynbuf *buf, char *line, size_t *i, t_minishell *ms);
int			ft_lex_space(t_dynbuf *b, t_token **lst, char *l, size_t *i);
int 		ft_lex_loop(t_lexctx *c, char *line);
t_token		*ft_lexer(char *line, t_minishell *ms);

/* PARSER.C */

t_command	*ft_init_cmd(void);
int			ft_argv_push(t_command *cmd, char *word);
int			parse_word(t_command *cmd, t_token *tok);
int			parse_pipe(t_command **cur, int *err);
int			parse_redir(t_command *cmd, t_token **tok, int *err);
t_command	*parse_tokens(t_token *tok, int *err);
int			redir_push(t_command *cmd, t_token_type type, char *file);

/* TOKEN.C */

int			ft_add_token(t_token **list, t_token_type type, char *value);
int			ft_found_operator_token(t_token **list, char *line, size_t *i);
void		ft_token_clear(t_token **lst);

/* SIGNAL.C */

void		ft_sigint_prompt(int sig);
void		ft_setup_prompt_signals(void);
void		ft_setup_child_signals(void);

/* ENV.C */

t_env		*ft_env_new(char *key, char *val);
int			ft_env_add_back(t_env **head, t_env *new);
int			ft_split_key_value(char *src, char **key, char **val);
t_env		*init_env(char **envp);
void		ft_free_env_list(t_env *head);

#endif