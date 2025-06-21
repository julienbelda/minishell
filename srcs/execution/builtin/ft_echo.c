#include "minishell.h"

// echo -> je dois afficher les arguments separé par un espace

void builtin_echo(char **argv)
{
    int i;

	i = 1;
	while (argv[i])
	{
		printf("%s", argv[i]);
		if (argv[i + 1])
			printf(" ");
		i++;
	}
	printf("\n");
}


int main(int argc, char **argv)
{
	if (argc < 2)
	{
		printf("nombres args incorrect");
		return 0;
	}
	else
	{
		if (ft_strcmp(argv[1], "echo") == 0)
			builtin_echo(argv + 1);
		else
		{
			printf("commande inconnu");
			return 0;
		}
	}
}