#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include "libft.h"
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
	char				*name; //Pointe vers argv[0] utiles pour les builtins.
	char				**args; //Tableau d'args déstiné à exceve.
	t_redirection		*redirection;
	struct s_command	*next;
	struct s_command	*prev;
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
}	t_dynbuf

extern volatile sig_atomic_t	g_signal;

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

/* LEXER */

void	ft_dynbuf_init(t_dynbuf *b);
int		ft_buf_grow(t_dynbuf *b, size_t need);
int		ft_dynbuf_add_char(t_dynbuf *b, char c);
char	*ft_dynbuf_str(t_dynbuf *b);
void	ft_dynbuf_free(t_dynbuf *b);

#endif