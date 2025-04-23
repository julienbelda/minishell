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

typedef struct s_token
{
	t_token_type	type; //WORD, PIPE..
	char			*value; // Mot, nom d'un fichier, délimiteur..
	struct s_token	*next;
	struct s_token	*prev;

}	t_token;

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
	char			*key;
	char			*value;
	bool			is_exported; //1 visible dans l'env
	struct s_env	*next;
	struct s_env	*prev;
}	t_env;

typedef struct s_redirection
{
	t_token_type	type; //REDIR_IN, REDIR_OUT, REDIR_APPEND, HEREDOC..
	char			*file; //File ou délim si heredoc
}	t_redirection;

typedef struct s_minishell
{
	t_env	*env;
	int		last_status; //Pour $?
}	t_minishell;

#endif