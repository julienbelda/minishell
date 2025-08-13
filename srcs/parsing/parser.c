#include "minishell.h"

t_command	*ft_init_cmd(void)
{
	t_command	*cmd;

	cmd = malloc(sizeof(*cmd));
	if (!cmd)
		return (NULL);
	cmd->name = NULL;
	cmd->argv_cap = 4;
	cmd->argv_full_cap = 4;
	cmd->argv = malloc(sizeof(char *) * cmd->argv_cap);
	cmd->argv_full = malloc(sizeof(char *) * cmd->argv_full_cap);
	if (!cmd->argv || !cmd->argv_full)
		return (free(cmd->argv), free(cmd->argv_full), free(cmd), NULL);
	cmd->argc = 0;
	cmd->argc_full = 0;
	cmd->argv[0] = NULL;
	cmd->argv_full[0] = NULL;
	cmd->redir = NULL;
	cmd->next = NULL;
	cmd->assign_count = 0;
	return (cmd);
}

static int	push_to_argv(t_command *cmd, char *word)
{
	char	**new;
	size_t	i;

	if (cmd->argc + 1 >= cmd->argv_cap)
	{
		cmd->argv_cap *= 2;
		new = malloc(sizeof(char *) * cmd->argv_cap);
		if (!new)
			return (-1);
		i = 0;
		while (i < (size_t)cmd->argc)
		{
			new[i] = cmd->argv[i];
			i++;
		}
		free(cmd->argv);
		cmd->argv = new;
	}
	cmd->argv[cmd->argc++] = word;
	cmd->argv[cmd->argc] = NULL;
	return (0);
}

static int	push_to_argv_full(t_command *cmd, char *word)
{
	char	**new;
	size_t	i;

	if (cmd->argc_full + 1 >= cmd->argv_full_cap)
	{
		cmd->argv_full_cap *= 2;
		new = malloc(sizeof(char *) * cmd->argv_full_cap);
		if (!new)
			return (-1);
		i = 0;
		while (i < (size_t)cmd->argc_full)
		{
			new[i] = cmd->argv_full[i];
			i++;
		}
		free(cmd->argv_full);
		cmd->argv_full = new;
	}
	cmd->argv_full[cmd->argc_full++] = word;
	cmd->argv_full[cmd->argc_full] = NULL;
	return (0);
}

int	ft_argv_push(t_command *cmd, char *word)
{
	if (push_to_argv(cmd, word) == -1)
		return (-1);
	if (push_to_argv_full(cmd, word) == -1)
		return (-1);
	return (0);
}

int	parse_word(t_command *cmd, t_token *tok)
{
	if (!cmd->name)
		cmd->name = tok->value;
	if (ft_argv_push(cmd, tok->value) == -1)
		return (-1);
	return (0);
}
