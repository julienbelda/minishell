#include "minishell.h"

void free_env(t_env *env)
{
    t_env *temp; // je peux pas free et env->next apres car acces mémoire invalide
    
	temp = env;
    if (!env)
		return;
    while (env)
    {   
        temp = env->next;
        // avant de free je dois stocker le node dans temp
        free(env->key);
		free(env->value);
		free(env); // free actuel
        // redonner a env la value de temp
        env = temp;
    }
}

void free_envp(char **envp)
{
    int i;

    if (!envp)
        return ;
    i = 0;
    while (envp[i])
        free(envp[i++]);
    free(envp);
}

void    ft_split_free(char **tab)
{
    int i;

    if (!tab)
        return ;
    i = 0;
    while (tab[i])
    {
        free(tab[i]);
        i++;
    }
    free(tab);
}