#include "minishell.h"

int builtin_env(t_minishell *mini)
{
	t_env *temp;
	temp = mini->env; // je taff sur une copie de ma liste de var
	while (temp)
	{
		if (temp->is_exported && temp->value) // is exported vérifie si la var est visible && temp->value car on ne print pas si la value est null 
			printf("%s=%s\n", temp->key, temp->value);
		temp = temp->next;
	}
	set_exit_status(mini, 0, NULL);
	return 0;
}

/* t_env *create_env_node(char *key, char *value, bool is_exported)
{
	t_env *node = malloc(sizeof(t_env));
	if (!node)
		return NULL;
	node->key = key ? strdup(key) : NULL;
	node->value = value ? strdup(value) : NULL;
	node->is_exported = is_exported;
	node->next = NULL;
	node->prev = NULL;
	return node;
} */

/* int main(void)
{
	t_env *a = create_env_node("PATH", "/bin", true);
	t_env *b = create_env_node("FOO", "hello", false);
	t_env *c = create_env_node("EMPTY", NULL, true);

	a->next = b;
	b->next = c;
	c->next = NULL;

	printf("===== Résultat de builtin_env =====\n");
	builtin_env(a);
}
 */