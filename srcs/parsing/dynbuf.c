#include "minishell.h"

/* Met le tampon à zéro (pointeur NULL, len = 0, cap = 0). */

void	ft_dynbuf_init(t_dynbuf *b)
{
	b->data = NULL;
	b->len = 0;
	b->capacity = 0;
}

/* 	Agrandit data si len + need dépasse cap.
	- capacité min 32, puis ×2 jusqu’à suffisant.
	- copie l’ancien contenu et libère l’ancien bloc. */

int	ft_buf_grow(t_dynbuf *b, size_t need)
{
	size_t	new_capacity;
	char	*new;

	if (b->len + need <= b->capacity)
		return (0);
	new_capacity = b->capacity;
	if (new_capacity == 0)
		new_capacity = 32;
	while (new_capacity < b->capacity + need)
		new_capacity *= 2;
	new = malloc(new_capacity + 1);
	if (!new)
		return (-1);
	if (b->data)
		ft_memcpy(new, b->data, b->len);
	free(b->data);
	b->data = new;
	b->capacity = new_capacity;
	return (0);
}

/* 	Ajoute un caractère :
	1. vérifie l’espace (ft_buf_grow).
	2. écrit le char.
	3. maintient le '\0' final.
*/

int	ft_dynbuf_add_char(t_dynbuf *b, char c)
{
	if (ft_buf_grow(b, 1) == -1)
		return (-1);
	b->data[b->len++] = c;
	b->data[b->len] = '\0';
	return (0);
}

/* 	Duplique le contenu actuel (len) → nouvelle chaîne malloc-ée.
	Si len == 0 → retourne NULL.
*/
char	*ft_dynbuf_str(t_dynbuf *b)
{
	char	*out;

	if (b->len == 0)
		return (NULL);
	out = malloc(b->len + 1);
	if (!out)
		return (NULL);
	ft_memcpy(out, b->data, b->len + 1);
	return (out);
}

/* libère data et remet la struct à zéro. */
void	ft_dynbuf_free(t_dynbuf *b)
{
	free(b->data);
	b->data = NULL;
	b->len = 0;
	b->capacity = 0;
}
