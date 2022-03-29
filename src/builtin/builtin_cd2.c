/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkosaka <jkosaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/29 19:21:10 by jkosaka           #+#    #+#             */
/*   Updated: 2022/03/29 19:22:31 by jkosaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	remove_last_dir(char **update_relative_path)
{
	char	*target;

	target = ft_strrchr(*update_relative_path, '/');
	if (target != *update_relative_path)
		*target = '\0';
	else
		*(target + 1) = '\0';
}

void	update_relative_path(char *old_pwd, char *path_name, \
											 t_sh_var *sh_var)
{
	char	*update_relative_path;
	char	**splitted_str;
	char	**head;
	size_t	len;

	len = ft_strlen(old_pwd) + ft_strlen(path_name) + 2;
	update_relative_path = ft_xcalloc(len, sizeof(char));
	ft_strlcat(update_relative_path, old_pwd, len);
	splitted_str = ft_xsplit(path_name, '/');
	head = splitted_str;
	while (*splitted_str)
	{
		if (ft_strcmp(*splitted_str, "..") == 0)
			remove_last_dir(&update_relative_path);
		else if (ft_strcmp(*splitted_str, ".") != 0)
		{
			ft_strlcat(update_relative_path, "/", len);
			ft_strlcat(update_relative_path, *splitted_str, len);
		}
		splitted_str++;
	}
	ft_split_free(head);
	free(old_pwd);
	sh_var->pwd = update_relative_path;
}

void	update_absolute_path(char *path_name, t_sh_var *sh_var)
{
	int	i;

	i = 0;
	if (ft_strncmp(path_name, "///", 3) == 0)
	{
		while (path_name[i] == '/' && path_name[i + 1] == '/')
			i++;
	}
	sh_var->pwd = ft_xstrdup(&(path_name[i]));
}
