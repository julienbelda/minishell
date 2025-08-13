#include "minishell.h"

bool	print_error(char *str)
{
	if (str)
		write(2, str, ft_strlen(str));
	return (true);
}

void	set_exit_status(t_minishell *mini, int code, const char *msg)
{
	mini->last_status = code;
	if (msg)
		fprintf(stderr, "%s\n", msg);
}
