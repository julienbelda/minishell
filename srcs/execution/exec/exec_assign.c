#include "minishell.h"

static bool	is_assignment_str(const char *s)
{
	if (!s || !ft_strchr(s, '='))
		return (false);
	if (*s == '=')
		return (false);
	return (ft_strchr(s, '/') == NULL);
}

int	handle_assignments(t_command *cmd, t_minishell *ms)
{
	int		i;
	int		j;
	char	*eq;

	i = 0;
	while (cmd->argv[i] && is_assignment_str(cmd->argv[i]))
	{
		eq = ft_strchr(cmd->argv[i], '=');
		*eq = '\0';
		set_env(&ms->env, cmd->argv[i], eq + 1);
		*eq = '=';
		i++;
	}
	if (cmd->argv[i] == NULL)
	{
		ms->last_status = 0;
		return (0);
	}
	if (i > 0)
	{
		j = 0;
		while (cmd->argv[i])
			cmd->argv[j++] = cmd->argv[i++];
		cmd->argv[j] = NULL;
	}
	cmd->name = cmd->argv[0];
	return (1);
}
