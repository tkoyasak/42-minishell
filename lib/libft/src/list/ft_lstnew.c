/*
** Allocates (with malloc) new element.
** The variable 'content' is initialized with the value of parameter 'content'.
** The variable 'next' is initialized to NUL.
** Returns the new element.
*/

#include "libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*ret;

	ret = malloc(sizeof(t_list));
	if (!ret)
		return (NULL);
	ret->content = content;
	ret->next = NULL;
	return (ret);
}
