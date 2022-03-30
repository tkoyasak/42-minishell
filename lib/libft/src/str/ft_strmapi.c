/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkosaka <jkosaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/30 15:52:27 by jkosaka           #+#    #+#             */
/*   Updated: 2022/03/30 15:52:28 by jkosaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** Applies 'f' to each character of 's' to create new string (with malloc)
** resulting from successive applications of 'f'.
** Returns string created from the successive applications of 'f',
** or returns NUL if the allocation fails.
*/

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char	*ret;
	size_t	slen;
	size_t	i;

	if (!s || !f)
		return (NULL);
	slen = ft_strlen(s);
	ret = malloc(sizeof(char) * (slen + 1));
	if (!ret)
		return (NULL);
	i = 0;
	while (i < slen)
	{
		ret[i] = (*f)(i, s[i]);
		i++;
	}
	ret[i] = '\0';
	return ((char *)ret);
}
