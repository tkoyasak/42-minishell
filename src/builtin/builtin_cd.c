/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkosaka <jkosaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 13:57:29 by jkosaka           #+#    #+#             */
/*   Updated: 2022/03/29 17:35:53 by jkosaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	cd_error(char *path_name)
{
	ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
	perror(path_name);
}

static void	remove_last_dir(char **update_relative_path)
{
	char	*target;

	target = ft_strrchr(*update_relative_path, '/');
	if (target != *update_relative_path)
		*target = '\0';
	else
		*(target + 1) = '\0';
}

static void	update_relative_path(char *old_pwd, char *path_name, t_sh_var *sh_var)
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

static void	update_absolute_path(char *path_name, t_sh_var *sh_var)
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

static int	builtin_cd_pwd_update(char *path_name, t_sh_var *sh_var)
{
	char	*relative_path;
	char	*cwd_path;

	free(sh_var->oldpwd);
	sh_var->oldpwd = sh_var->pwd;
	cwd_path = getcwd(NULL, 0);
	if (cwd_path == NULL)
	{
		ft_putendl_fd("minishell: cd: error retrieving current directory: \
			getcwd: cannot access parent directories: \
			No such file or directory", STDERR_FILENO);
		relative_path = ft_xstrjoin("/", path_name);
		sh_var->pwd = ft_xstrjoin_free(sh_var->pwd, relative_path, true);
	}
	else if (*path_name == '/')
		update_absolute_path(path_name, sh_var);
	else
		update_relative_path(ft_xstrdup(sh_var->pwd), path_name, sh_var);
	free(path_name);
	free(cwd_path);
	set_env_value("OLDPWD", sh_var->oldpwd, sh_var);
	set_env_value("PWD", sh_var->pwd, sh_var);
	return (0);
}

int	builtin_cd(t_proc *proc, t_sh_var *sh_var)
{
	char	*path_name;

	path_name = NULL;
	if (proc->command[1])
		path_name = ft_xstrdup(proc->command[1]);
	if (path_name == NULL)
	{
		path_name = get_env_value("HOME", sh_var);
		if (path_name == NULL)
		{
			ft_putendl_fd("minishell: cd: HOME not set", STDERR_FILENO);
			return (1);
		}
		if (*path_name == '\0')
			return (free(path_name), 0);
	}
	if (chdir(path_name) == -1)
	{
		cd_error(path_name);
		free(path_name);
		return (1);
	}
	return (builtin_cd_pwd_update(path_name, sh_var));
}
