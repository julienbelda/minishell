#include "minishell.h"

int	parse_option(char *s)
{
	int	i;

	i = 1;
	if (!s || s[0] != '-' || s[1] != 'n')
		return (0);
	while (s[i] == 'n')
		i++;
	return (s[i] == '\0');
}

int	builtin_echo(char **argv, t_minishell *ms)
{
	(void) ms;
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	if (argv[i] && parse_option(argv[i]))
	{
		newline = 0;
		while (argv[i] && parse_option(argv[i]))
			i++;
	}
	while (argv[i])
	{
		printf("%s", argv[i]);
		if (argv[i + 1])
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
	return (0);
}
