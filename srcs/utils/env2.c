#include "minishell.h"

char *create_env_with_kv(const char *key, const char *value)
{
    t_env *new_nodes;
    new_nodes = malloc(sizeof(t_env));
    if (!new_nodes)
        return NULL;
    new_nodes->key = ft_strdup(key);
    if (!new_nodes->key)
    {
        free(new_nodes);
        return NULL;
    }
    new_nodes->value = ft_strdup(value);
    if (!new_nodes->value)
    {
        free(new_nodes->key);
        free(new_nodes);
        return NULL;
    }
    new_nodes->next = NULL;
    return (new_nodes);
}