#include "minishell.h"

bool	is_assignment_str(const char *s)
{
	if (!s || !ft_strchr(s, '='))
		return (false);
	if (*s == '=')
		return (false);
	return (ft_strchr(s, '/') == NULL);
}

static int	handle_only_assignments(t_minishell *ms)
{
	ms->last_status = 0;
	return (0);
}

int	handle_assignments(t_command *cmd, t_minishell *ms)
{
	int	i;

	i = 0;
	while (cmd->argv[i] && is_assignment_str(cmd->argv[i]))
		i++;
	cmd->assign_count = i;
	if (cmd->argv[i] == NULL)
		return (handle_only_assignments(ms));
	return (1);
}
