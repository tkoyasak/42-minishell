/*
** Frees the memory of element’s 'content' using function 'del'.
** Frees element 'lst'.
** The memory of 'next' must not be freed.
*/

#include "libft.h"

void	ft_lstdelone(t_list *lst, void (*del)(void *))
{
	if (!lst)
		return ;
	if (*del)
		del(lst->content);
	free(lst);
}
