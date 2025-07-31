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

#define ERR_MALLOC "Erreur malloc\n"

#define COLOR_MAGENTA_BRIGHT "\033[95m"
#define COLOR_RESET          "\033[0m"

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
	int						fd;
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

typedef struct s_fd
{
    int in;
    int out;
}   t_fd;

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
char **env_to_array(t_env *env);
void free_env_array(char **arr);

int	lex_operator(t_dynbuf *b, t_token **lst, char *l, size_t *i);
int	lex_quote_toggle(t_lstate *s, char *l, size_t *i);
int	lex_variable(t_dynbuf *b, char *l, size_t *i, t_minishell *ms);
int	lex_space(t_dynbuf *b, t_token **lst, char *l, size_t *i);
int  lex_loop(t_lexctx *c, char *line);
t_token *ft_lexer(char *line, t_minishell *ms);
int	ft_add_token(t_token **list, t_token_type type, char *value);

bool	print_error(char *str);
int print_sorted_env(t_env *env);
void sort_env_array(t_env **array, int size);
void ft_env_add_back2(t_env **env, t_env *new);
int	ft_strcmp(const char *s1, const char *s2);
void set_exit_status(t_minishell *mini, int code, const char *msg);
void free_env(t_env *env);
void remove_node_target(t_env **head, t_env *target);
char *get_env(t_env *env, const char *key);
t_env *create_env_with_kv(const char *key, const char *value);
int set_env(t_env **env, char *key,const char *value);

void    ft_split_free(char **tab);

/* FT_CD.C */

int update_var_oldpwd_pwd(t_env **env);
int handle_home_cd(t_env **env);
int handle_oldpwd(t_env **env);
int ft_cd(char **argv, t_minishell *ms);

/*FT_ECHO.C */

int  parse_option(char *s);
int builtin_echo(char **argv, t_minishell *ms);

/* FT_ENV.C */

int ft_env(char **argv, t_minishell *ms);
void free_envp(char **envp);

/* FT_EXIT.C */

int ft_is_numeric(char *str);
int builtin_exit(char **args, t_minishell *mini);

/* FT_EXPORT.C */

bool valid_identifier(const char *str);
t_env *find_env(t_env *env, const char *key);
int export_assign(t_minishell *mini, char *key, char *value);
int export_single(t_minishell *mini, const char *arg);
int ft_export(char **args, t_minishell *mini);

/* FT_PWD.C */

int ft_pwd(char **argv, t_minishell *ms);

/* FT_UNSET.C */

int builtin_unset(char **args, t_minishell *mini);


/* FT_HELPER.C */

bool is_builtin(t_command *cmd);
int run_builtin(t_command *cmd, t_minishell *ms);

/* PATH.C */

char *join_path(const char *dir, const char *file);
char *get_cmd_path(char *name, t_env *env);

/* REDIRECTION. C */

int redir_in(const char *file);
int redir_out(const char *file);
int redir_append(const char *file);
int apply_redir(t_command *cmds);
int apply_redirections(t_redirection *redir);

/*EXEC.C*/

pid_t launch_child(t_command *cmd, int in_fd, int out_fd, t_minishell *ms);
int handle_assignments(t_command *cmd, t_minishell *ms);

int process_heredocs(t_command *cmds, t_minishell *ms);
char    *ft_expand_line(char *raw, t_minishell *ms);
int ft_exec_heredoc(char *eof, int *fdread, t_minishell *ms, bool expand);
char	*ft_expand_line(char *raw, t_minishell *ms);

void	ft_restore_prompt_signals(void);
void ft_setup_heredoc_signals();
#endif