/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkosaka <jkosaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/30 15:52:45 by jkosaka           #+#    #+#             */
/*   Updated: 2022/03/30 15:52:46 by jkosaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** Locates last 'c'(converted to char) occurring in string pointed to by 's'.
** The terminating NUL character is considered to be part of the string
** (therefore locates terminating `\0' if c is `\0').
** Returns a pointer to the located character,
** or NUL if character 'c' does not appear in 's'.
*/

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	*p;

	p = (char *)s + ft_strlen(s);
	while (p != s && *p != (char)c)
		p--;
	if (*p == (char)c)
		return (p);
	return (NULL);
}
