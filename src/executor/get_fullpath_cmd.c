/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_fullpath_cmd.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkosaka <jkosaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 11:16:50 by jkosaka           #+#    #+#             */
/*   Updated: 2022/03/28 21:47:18 by jkosaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_str(char **str)
{
	if (str && (*str))
	{
		free(*str);
		*str = NULL;
	}
}

static void	cmd_not_found(char *cmd)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putendl_fd(": command not found: ", STDERR_FILENO);
}

static char	*get_fullcmd_core(char *cmd, char **all_paths)
{
	int		path_index;
	char	*fullcmd;
	char	*temp;

	path_index = -1;
	while (all_paths[++path_index])
	{
		temp = ft_xstrjoin(all_paths[path_index], "/");
		fullcmd = ft_xstrjoin_free(temp, cmd, false);
		if (!(access(fullcmd, X_OK)))
			return (fullcmd);
		free_str(&fullcmd);
	}
	cmd_not_found(cmd);
	return (NULL);
}

/*  return NULL if command not found  */
char	*get_fullpath_cmd(char *cmd, t_sh_var *sh_var)
{
	char	**all_paths;
	char	*path_env;

	if (!cmd)
		exit(EXIT_FAILURE);
	if (!cmd[0])
		return (cmd);
	if (!access(cmd, X_OK))
	{
		if (ft_strchr(cmd, '/') == NULL)
		{
			cmd_not_found(cmd);
			return (NULL);
		}
		return (cmd);
	}
	path_env = get_env_value_str("PATH", sh_var);
	if (!path_env)
	{
		cmd_not_found(cmd);
		return (NULL);
	}
	all_paths = ft_xsplit(path_env, ':');
	return (get_fullcmd_core(cmd, all_paths));
}
