#include "minishell.h"

// pwd -> afficher le chemin ou je suis

int builtin_pwd(t_minishell *mini)
{
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)))
	{
		printf("%s\n", cwd);
		set_exit_status(mini, 0, NULL);
		return 0;
	}
    else
		set_exit_status(mini, 1, "error pwd");
	return 1;
}

int main(int argc, char **argv)
{
    if (argc < 2)
        return 0;
	t_minishell mini;
	mini.env = NULL;
	mini.last_status = 0;
	if (ft_strcmp(argv[1], "pwd") == 0)
	{
		builtin_pwd(&mini);
		printf("le code de sortie est : %d\n", mini.last_status);
	}
	return 0; 
}