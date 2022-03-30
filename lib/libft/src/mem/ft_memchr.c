/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkosaka <jkosaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/30 15:50:49 by jkosaka           #+#    #+#             */
/*   Updated: 2022/03/30 15:50:50 by jkosaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** Locates first occurrence of 'c'(converted to unsigned char) in 's'.
** Returns a pointer to the byte located, or NULL if no such byte exists
** within 'n' bytes.
*/

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*p;

	p = (unsigned char *)s;
	while (n--)
		if (*p++ == (unsigned char)c)
			return (--p);
	return (NULL);
}
