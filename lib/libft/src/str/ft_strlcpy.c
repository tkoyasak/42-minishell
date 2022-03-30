/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkosaka <jkosaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/30 15:52:18 by jkosaka           #+#    #+#             */
/*   Updated: 2022/03/30 15:52:19 by jkosaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** Copies up to 'dstsize - 1' characters from 'src' to 'dst'.
** Take full size of the destination buffer.
** Guarantee NUL-termination if 'dstsize' is not zero.
** (room for NUL should be included in 'dstsize'.)
** Returns total length of string they tried to create.
** :length of 'src'
** If return value is >= 'dstsize', output string has been truncated.
*/

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	srclen;

	srclen = ft_strlen(src);
	if (srclen + 1 < dstsize)
		ft_memcpy(dst, src, srclen + 1);
	else if (dstsize)
	{
		ft_memcpy(dst, src, dstsize - 1);
		*(dst + dstsize - 1) = '\0';
	}
	return (srclen);
}
