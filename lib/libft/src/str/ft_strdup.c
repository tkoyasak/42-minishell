/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkosaka <jkosaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/30 15:51:54 by jkosaka           #+#    #+#             */
/*   Updated: 2022/03/30 15:51:55 by jkosaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** Allocates sufficient memory for a copy of string 's1'.
** Does the copy.
** Returns a pointer to it.
** If insufficient memory is available, NUL is returned
** (and errno is set to ENOMEM).
*/

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	char	*ret;
	size_t	len;

	len = ft_strlen(s1);
	ret = malloc(len + 1);
	if (!ret)
		return (NULL);
	ft_memcpy(ret, s1, len + 1);
	return (ret);
}
