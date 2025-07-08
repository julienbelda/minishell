/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minishell <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 17:10:00 by minishell         #+#    #+#             */
/*   Updated: 2025/07/04 17:10:00 by minishell        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* main.c ----------------------------------------------------------------- */
#include "minishell.h"
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

<<<<<<< HEAD
/* concatène argv[1..] en une seule chaîne, séparée par des espaces */
/* static char	*join_args(int ac, char **av)
=======
static void	process_line(char *line, t_minishell *ms)
>>>>>>> julien
{
	t_token		*toks;
	t_command	*cmds;
	int			err;

	if (!*line)                     /* ligne vide */
		return ;
	add_history(line);

	toks = ft_lexer(line, ms);
	if (!toks)
		return ;
	cmds = parse_tokens(toks, &err);
	if (!cmds)
	{
		printf("parse error (%d)\n", err);
		ft_token_clear(&toks);
		return ;
	}
	/* --- tant que l’exécuteur n’est pas prêt, on affiche --------------- */
	for (t_command *c = cmds; c; c = c->next)
		printf("[cmd] %s\n", c->name);
	/* free (helper à écrire) */
	ft_token_clear(&toks);
	/* free_cmd_list(cmds) … */
}
<<<<<<< HEAD
 */
/* programme de test : affiche la liste des tokens générés par ft_lexer */
/* int	main(int ac, char **av)
=======

int	main(int ac, char **av, char **envp)
>>>>>>> julien
{
	t_minishell	ms;
	char		*line;

	(void)ac; (void)av; (void)envp;
	ms.env = init_env(envp);
    if (!ms.env)
        return (perror("env"), 1);
	ms.last_status = 0;

	ft_setup_prompt_signals();
	while (1)
	{
		line = readline("minishell$ ");
		if (!line)               /* Ctrl-D ou EOF */
		{
			printf("exit\n");
			break ;
		}
		process_line(line, &ms);
		free(line);
	}
	/* free env… */
	return (0);
}
 */