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
	char	**args;

	args = cmd->argv + cmd->assign_count;
	if (!ft_strcmp(cmd->name, "echo"))
		return (builtin_echo(args, ms));
	if (!ft_strcmp(cmd->name, "cd"))
		return (ft_cd(args, ms));
	if (!ft_strcmp(cmd->name, "pwd"))
		return (ft_pwd(args, ms));
	if (!ft_strcmp(cmd->name, "export"))
		return (ft_export(args, ms));
	if (!ft_strcmp(cmd->name, "unset"))
		return (builtin_unset(args, ms));
	if (!ft_strcmp(cmd->name, "env"))
		return (ft_env(args, ms));
	if (!ft_strcmp(cmd->name, "exit"))
		return (builtin_exit(args, ms));
	return (1);
}

int	run_builtin_env(t_command *cmd, t_minishell *ms, t_env *env_override)
{
	t_minishell	ms_tmp;

	if (!env_override)
		return (run_builtin(cmd, ms));
	ms_tmp = *ms;
	ms_tmp.env = env_override;
	return (run_builtin(cmd, &ms_tmp));
}
