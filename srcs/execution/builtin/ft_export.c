#include "minishell.h"

static bool valid_identifier(const char *str)
{
	int i = 0;
	if (!str[0] || (!ft_isalpha(str[0]) && str[0] != '_'))
		return false;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return false;
		i++;
	}
	return true;
}

static t_env *find_env(t_env *env, const char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return env;
		env = env->next;
	}
	return NULL;
}

static int export_assign(t_minishell *mini, char *key, char *value)
{
	t_env *existing = find_env(mini->env, key);
	if (existing)
	{
		free(existing->value);
		existing->value = value;
		existing->is_exported = true;
		return 0;
	}
	t_env *new = malloc(sizeof(t_env));
	if (!new)
		return (free(key), free(value), print_error(ERR_MALLOC));
	new->key = key;
	new->value = value;
	new->is_exported = true;
	new->next = NULL;
	new->prev = NULL;
	ft_env_add_back(&mini->env, new);
	return 0;
}

static int export_single(t_minishell *mini, const char *arg)
{
	char *eq = ft_strchr(arg, '=');
	char *key = NULL, *value = NULL;
	if (!valid_identifier(arg))
		return (write(2, "export: invalid identifier\n", 28), 1);
	if (!eq)
		return 0;
	key = ft_substr(arg, 0, eq - arg);
	value = ft_strdup(eq + 1);
	if (!key || !value)
		return (free(key), free(value), print_error(ERR_MALLOC));
	return export_assign(mini, key, value);
}

int ft_export(char **args, t_minishell *mini)
{
	int i = 1;
	int status = 0;
	if (!args[1])
		return print_sorted_env(mini->env);
	while (args[i])
	{
		if (export_single(mini, args[i]))
			status = 1;
		i++;
	}
	return status;
}

int main(void)
{
	t_minishell mini;
	t_env *env = NULL;

	mini.env = env;
	mini.last_status = 0;

	char *cmd1[] = {"export", "VAR=hello", NULL};
	ft_export(cmd1, &mini);

	char *cmd2[] = {"export", "PATH=/bin", NULL};
	ft_export(cmd2, &mini);

	char *cmd3[] = {"export", NULL};
	ft_export(cmd3, &mini);
	return 0;
}


