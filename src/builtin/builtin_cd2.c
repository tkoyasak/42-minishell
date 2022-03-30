/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkosaka <jkosaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/29 19:21:10 by jkosaka           #+#    #+#             */
/*   Updated: 2022/03/30 22:51:44 by jkosaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	remove_last_dir(char **updated_path)
{
	char	*target;

	target = ft_strrchr(*updated_path, '/');
	if (!target)
	{
		(*updated_path)[0] = '/';
		(*updated_path)[1] = '\0';
	}
	else if (target != *updated_path)
		*target = '\0';
	else
		*(target + 1) = '\0';
}

static void	concatenate_path(char **updated_path, char *splitted_str, \
													size_t len)
{
	if (ft_strcmp(splitted_str, "..") == 0)
		remove_last_dir(updated_path);
	else if (ft_strcmp(splitted_str, ".") != 0)
	{
		if (!((*updated_path)[0]) || \
				(*updated_path)[ft_strlen(*updated_path) - 1] != '/')
			ft_strlcat(*updated_path, "/", len);
		ft_strlcat(*updated_path, splitted_str, len);
	}
}

static char	*search_from_relative_path(char *old_pwd, char *path_name)
{
	char	*updated_path;
	char	**splitted_str;
	char	**head;
	size_t	len;

	len = ft_strlen(old_pwd) + ft_strlen(path_name) + 2;
	updated_path = ft_xcalloc(len, sizeof(char));
	ft_strlcat(updated_path, old_pwd, len);
	splitted_str = ft_xsplit(path_name, '/');
	head = splitted_str;
	while (*splitted_str)
	{
		concatenate_path(&updated_path, *splitted_str, len);
		splitted_str++;
	}
	ft_split_free(head);
	free(old_pwd);
	if (ft_strcmp(updated_path, "") == 0)
		ft_strlcat(updated_path, "/", len);
	return (updated_path);
}

static char	*search_from_absolute_path(char *path_name)
{
	int		idx;

	idx = 0;
	if (ft_strncmp(path_name, "///", 3) == 0)
	{
		while (path_name[idx] == '/' && path_name[idx + 1] == '/')
			idx++;
	}
	if (ft_strcmp(&(path_name[idx]), "/") == 0 || \
				ft_strcmp(&(path_name[idx]), "//") == 0)
	{
		return (ft_xstrdup(&(path_name[idx])));
	}
	return (search_from_relative_path(ft_xstrdup(""), &path_name[idx + 1]));
}

char	*cd_dst_path(char *path_name, t_sh_var *sh_var)
{
	char	*no_dot_pwd_path;
	char	*dst_path;

	if (*path_name == '/')
		dst_path = search_from_absolute_path(path_name);
	else
	{
		no_dot_pwd_path = search_from_absolute_path(sh_var->pwd);
		dst_path = search_from_relative_path(no_dot_pwd_path, path_name);
	}
	return (dst_path);
}
