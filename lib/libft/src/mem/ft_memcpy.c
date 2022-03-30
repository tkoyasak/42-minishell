/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkosaka <jkosaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/30 15:50:59 by jkosaka           #+#    #+#             */
/*   Updated: 2022/03/30 15:51:00 by jkosaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** Copies 'n' bytes from memory area 'src' to memory area 'dst'.
** Behavior is undefined if 'dst' and 'src' overlap.
** Returns original value of 'dst'.
*/

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	unsigned char	*d;
	unsigned char	*s;

	if (n == 0 || dst == src)
		;
	else
	{
		d = (unsigned char *)dst;
		s = (unsigned char *)src;
		while (n--)
			*d++ = *s++;
	}
	return ((void *)dst);
}
