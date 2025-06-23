#include "minishell.h"

void remove_node_target(t_env **head, t_env *target)
{
    if (!*head || !target || !head)
        return;
    if (target->prev)
        target->prev->next = target->next;
    else
        *head = target->next;
    
    if (target->next)
        target->next->prev = target->prev;
    free(target->key);
    free(target->value);
    free(target);
}
