#include "minishell.h"
#include <stdio.h>

static void	free_redirs(t_redirection *r)
{
	t_redirection	*tmp;

	while (r)
	{
		tmp = r->next;
		free(r);
		r = tmp;
	}
}

static void	free_cmd_list(t_command *cmd)
{
	t_command	*tmp;

	while (cmd)
	{
		tmp = cmd->next;
		free(cmd->argv);
		free_redirs(cmd->redir);
		free(cmd);
		cmd = tmp;
	}
}

static void	process_line(char *line, t_minishell *ms)
{
	t_token		*toks;
	t_command	*cmds;
	int			err;

	if (!*line)
		return ;
	add_history(line);
	toks = ft_lexer(line, ms);
	if (!toks)
		return ;
	cmds = parse_tokens(toks, &err);
	if (!cmds)
	{
		fprintf(stderr, "minishell: parse error\n");
		ft_token_clear(&toks);
		return ;
	}
	ft_execute(cmds, ms);
	ft_token_clear(&toks);
	free_cmd_list(cmds);
}

int	main(int ac, char **av, char **envp)
{
	t_minishell	ms;
	char		*line;

	(void)ac;
	(void)av;
	ms.env = init_env(envp);
	if (!ms.env)
		return (perror("minishell"), 1);
	ms.last_status = 0;
	ft_setup_prompt_signals();
	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
		{
			printf("exit\n");
			break ;
		}
		process_line(line, &ms);
		free(line);
	}
	ft_free_env_list(ms.env);
	rl_clear_history();
	return (ms.last_status);
}
