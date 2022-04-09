/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd_path.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkosaka <jkosaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 11:16:50 by jkosaka           #+#    #+#             */
/*   Updated: 2022/04/09 15:50:59 by jkosaka          ###   ########.fr       */
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

static bool	exists_command(char *path)
{
	struct stat	buf;

	if (!path)
		return (false);
	if (stat(path, &buf) == -1)
		return (false);
	if (S_ISDIR(buf.st_mode))
		return (false);
	return (true);
}

static char	*search_cmd_path(char *cmd, char **all_paths)
{
	int			path_index;
	char		*fullcmd;
	char		*cmd_candidate;
	char		*temp;
	struct stat	buf;

	fullcmd = NULL;
	path_index = -1;
	while (all_paths[++path_index])
	{
		temp = ft_xstrjoin(all_paths[path_index], "/");
		cmd_candidate = ft_xstrjoin_free(temp, cmd, false);
		if (exists_command(cmd_candidate))
		{
			free(fullcmd);
			fullcmd = ft_xstrdup(cmd_candidate);
			stat(fullcmd, &buf);
			if (S_ISREG(buf.st_mode))
			{
				ft_split_free(all_paths);
				return (fullcmd);
			}
		}
		free_str(&cmd_candidate);
	}
	print_error_msg(cmd, NO_CMD_MSG);
	ft_split_free(all_paths);
	exit(NO_CMD);
}

/*  return NULL if command not found  */
char	*get_cmd_path(char *cmd, t_sh_var *sh_var)
{
	char	**all_paths;
	char	*path_env;

	if (!cmd)
		exit(EXIT_FAILURE);
	if (!cmd[0])
		return (print_error_msg(cmd, NO_CMD_MSG), NULL);
	if (ft_strcmp(cmd, ".") == 0)
	{
		print_error_msg(".", "filename argument required");
		exit(2);
	}
	if (ft_strchr(cmd, '/'))
		return (cmd);
	path_env = get_env_value_str("PATH", sh_var);
	if (!path_env)
		return (print_error_msg(cmd, NO_CMD_MSG), NULL);
	all_paths = ft_xsplit(path_env, ':');
	return (search_cmd_path(cmd, all_paths));
}
