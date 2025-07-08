/* signal.c --------------------------------------------------------------- */
#include "minishell.h"
#include <readline/readline.h>

volatile sig_atomic_t	g_signal = 0;      /* autorisé : 1 globale */

/* ---- prompt : Ctrl-C doit juste redonner la main ---------------------- */
void	ft_sigint_prompt(int sig)
{
	(void)sig;
	g_signal = SIGINT;
	write(STDOUT_FILENO, "\n", 1);   /* force le retour à la ligne */
	rl_on_new_line();                /* informe readline           */
	rl_replace_line("", 0);          /* vide le buffer courant      */
	rl_redisplay();                  /* redessine le prompt         */
}
/* ---- initialisation des signaux pour le mode « lecture de ligne » ----- */
void	ft_setup_prompt_signals(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sa.sa_handler = ft_sigint_prompt;
	sigaction(SIGINT, &sa, NULL);   /* Ctrl-C   */
	signal(SIGQUIT, SIG_IGN);       /* Ctrl-\\  */
}

/* ---- pour un futur fork : rétablir comportement par défaut ------------ */
void	ft_setup_child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}
