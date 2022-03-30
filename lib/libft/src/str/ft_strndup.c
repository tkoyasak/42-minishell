/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkosaka <jkosaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/30 15:52:37 by jkosaka           #+#    #+#             */
/*   Updated: 2022/03/30 15:52:38 by jkosaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strndup(const char *s, size_t len)
{
	char	*ret;

	ret = malloc(len + 1);
	if (!ret)
		return (NULL);
	ft_memcpy(ret, s, len);
	*(ret + len) = '\0';
	return (ret);
}
