#include "minishell.h"

// echo -> je dois afficher les arguments separé par un espace

int parse_option(char *str)
{
	// checker si le deuxieme arg est l'option -n
	// DANS la fonction echo, si parse option est vrai -> je dois pas afficher de saut de ligne
	// il peut y avoir plusieurs n, exemple : -nnnnn fonctionne

	int i;
	
	i = 0;
	if (str[i] == '-')
	{
		i++;
		while (str[i] && str[i] == 'n')
		{
			i++;
			if (str[i] == '\0')
				return 1;
		}
		if (str[i] != 'n')
			return 0;
	}
	else
		return 0;
	return 0;
}

void builtin_echo(char **argv)
{
    int i;
	int newline;

	i = 1;
	newline = 1;
	if (parse_option(argv[1]))
	{
		i++;
		while (argv[i] && parse_option(argv[i]))
			i++;
		newline = 0;
	}
	while (argv[i])
	{
		printf("%s", argv[i]);
		if (argv[i + 1])
			printf(" ");
		i++;
	}
	if (!newline)
		return;
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