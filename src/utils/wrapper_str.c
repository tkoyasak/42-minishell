/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wrapper_str.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoyasak <tkoyasak@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/25 11:06:04 by tkoyasak          #+#    #+#             */
/*   Updated: 2022/03/25 11:06:05 by tkoyasak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_xstrdup(const char *s1)
{
	char	*ret;

	ret = ft_strdup(s1);
	if (!ret)
		error_handler("malloc");
	return (ret);
}

char	*ft_xstrndup(const char *s1, size_t len)
{
	char	*ret;

	ret = ft_strndup(s1, len);
	if (!ret)
		error_handler("malloc");
	return (ret);
}

char	*ft_xsubstr(char const *s, unsigned int start, size_t len)
{
	char	*ret;

	ret = ft_substr(s, start, len);
	if (!ret)
		error_handler("malloc");
	return (ret);
}

char	*ft_xstrjoin(char *s1, char *s2)
{
	char	*ret;

	ret = ft_strjoin(s1, s2);
	if (!ret)
		error_handler("malloc");
	return (ret);
}

char	*ft_xstrjoin_free(char *s1, char *s2, bool is_s2_free)
{
	char	*ret;

	ret = ft_strjoin_free(s1, s2, is_s2_free);
	if (!ret)
		error_handler("malloc");
	return (ret);
}
