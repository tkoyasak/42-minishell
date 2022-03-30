/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkosaka <jkosaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/30 15:51:17 by jkosaka           #+#    #+#             */
/*   Updated: 2022/03/30 15:51:18 by jkosaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* errno
** Contiguously allocates enough space for 'count' objects
** that are 'size' bytes of memory each.
** The allocated memory is filled with bytes of value zero.
** Returns a pointer to the allocated memory.
*/

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	void	*ret;

	if (count * size == 0)
	{
		count = 1;
		size = 1;
	}
	if (count > SIZE_MAX / sizeof(size))
	{
		errno = ENOMEM;
		return (NULL);
	}
	ret = malloc(count * size);
	if (!ret)
		return (NULL);
	ft_memset(ret, 0, count * size);
	return (ret);
}
