#include "minishell.h"

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minishell <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 15:20:00 by minishell         #+#    #+#             */
/*   Updated: 2025/06/02 15:20:00 by minishell        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>

/* concatène argv[1..] en une seule chaîne, séparée par des espaces */
/* static char	*join_args(int ac, char **av)
{
	int	i;
	size_t	len;
	char	*res;
	size_t	pos;

	i = 1;
	len = 0;
	while (i < ac)
		len += ft_strlen(av[i++]) + 1;
	if (!(res = malloc(len + 1)))
		return (NULL);
	i = 1;
	pos = 0;
	while (i < ac)
	{
		ft_memcpy(res + pos, av[i], ft_strlen(av[i]));
		pos += ft_strlen(av[i]);
		if (++i < ac)
			res[pos++] = ' ';
	}
	res[pos] = '\0';
	return (res);
}
 */
/* programme de test : affiche la liste des tokens générés par ft_lexer */
/* int	main(int ac, char **av)
{
	t_minishell	ms;
	t_token		*lst;
	t_token		*tmp;
	char		*line;

	ms.env = NULL;
	ms.last_status = 0;
	line = (ac > 1) ? join_args(ac, av)
		: ft_strdup("echo \"hello $USER\" | grep h >>out.txt");
	if (!line)
		return (perror("malloc"), 1);
	lst = ft_lexer(line, &ms);
	if (!lst)
		return (free(line), printf("lexer error\n"), 1);
	tmp = lst;
	while (tmp)
	{
		printf("%-10s : %s\n",
			(tmp->type == WORD) ? "WORD"
			: (tmp->type == PIPE) ? "PIPE"
			: (tmp->type == REDIR_IN) ? "<"
			: (tmp->type == REDIR_OUT) ? ">"
			: (tmp->type == REDIR_APPEND) ? ">>" : "<<",
			tmp->value ? tmp->value : "(null)");
		tmp = tmp->next;
	}
	ft_token_clear(&lst);
	free(line);
	return (0);
}
 */