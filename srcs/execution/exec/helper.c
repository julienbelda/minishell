#include "minishell.h"

bool	is_builtin(t_command *cmd)
{
	if (!cmd || !cmd->name)
		return (false);
	return (!ft_strcmp(cmd->name, "echo")
		|| !ft_strcmp(cmd->name, "cd")
		|| !ft_strcmp(cmd->name, "pwd")
		|| !ft_strcmp(cmd->name, "export")
		|| !ft_strcmp(cmd->name, "unset")
		|| !ft_strcmp(cmd->name, "env")
		|| !ft_strcmp(cmd->name, "exit"));
}

int	run_builtin(t_command *cmd, t_minishell *ms)
{
	if (!ft_strcmp(cmd->name, "echo"))
		return (builtin_echo(cmd->argv, ms));
	if (!ft_strcmp(cmd->name, "cd"))
		return (ft_cd(cmd->argv, ms));
	if (!ft_strcmp(cmd->name, "pwd"))
		return (ft_pwd(cmd->argv, ms));
	if (!ft_strcmp(cmd->name, "export"))
		return (ft_export(cmd->argv, ms));
	if (!ft_strcmp(cmd->name, "unset"))
		return (builtin_unset(cmd->argv, ms));
	if (!ft_strcmp(cmd->name, "env"))
		return (ft_env(cmd->argv, ms));
	if (!ft_strcmp(cmd->name, "exit"))
		return (builtin_exit(cmd->argv, ms));
	return (1);
}
