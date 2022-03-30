/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdelone.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkosaka <jkosaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/30 15:48:57 by jkosaka           #+#    #+#             */
/*   Updated: 2022/03/30 15:48:58 by jkosaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	if (del && lst->content)
	{
		del(lst->content);
		lst->content = NULL;
		lst->next = NULL;
	}
	free(lst);
	lst = NULL;
}
