/*
** Adds the element 'new' at the end of the list.
*/

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	if (!lst)
		return ;
	else if (!*lst)
		*lst = new;
	else
		(ft_lstlast(*lst))->next = new;
}
