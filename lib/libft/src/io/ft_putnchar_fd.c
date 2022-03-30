/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnchar_fd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkosaka <jkosaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/30 15:48:25 by jkosaka           #+#    #+#             */
/*   Updated: 2022/03/30 15:48:26 by jkosaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnchar_fd(char *s, size_t n, int fd)
{
	size_t	idx;

	idx = -1;
	while (++idx < n)
		ft_putchar_fd(s[idx], fd);
}
