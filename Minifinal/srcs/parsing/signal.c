/* signal.c --------------------------------------------------------------- */
#include "minishell.h"
#include <readline/readline.h>

volatile sig_atomic_t	g_signal = 0;

/* ---- prompt : Ctrl-C doit juste redonner la main ---------------------- */
void	ft_sigint_prompt(int sig)
{
	(void)sig;
	g_signal = SIGINT;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	ft_setup_prompt_signals(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sa.sa_handler = ft_sigint_prompt;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

/* ---- pour un futur fork : rétablir comportement par défaut ------------ */
void	ft_setup_child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGPIPE, SIG_DFL);
}

/* ---- heredoc : Ctrl-C doit interrompre le heredoc, pas le shell --------- */
void	ft_sigint_heredoc(int sig)
{
	(void)sig;
	g_signal = SIGINT;
	rl_replace_line("", 0);
	rl_on_new_line();
}

void	ft_setup_heredoc_signals(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sa.sa_handler = ft_sigint_heredoc;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

void	ft_restore_prompt_signals(void)
{
	ft_setup_prompt_signals();
}
