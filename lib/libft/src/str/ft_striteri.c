/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkosaka <jkosaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/30 15:51:59 by jkosaka           #+#    #+#             */
/*   Updated: 2022/03/30 15:52:00 by jkosaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** Applies function 'f' to each character of string passed as argument,
** and passing its index as first argument.
** Each character is passed by address to 'f' to be modified if necessary.
*/

#include "libft.h"

void	ft_striteri(char *s, void (*f)(unsigned int, char*))
{
	size_t	slen;
	size_t	i;

	if (!s || !f)
		return ;
	slen = ft_strlen(s);
	i = 0;
	while (i < slen)
	{
		f(i, (char *)s + i);
		i++;
	}
}
