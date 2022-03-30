/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkosaka <jkosaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 13:57:29 by jkosaka           #+#    #+#             */
/*   Updated: 2022/03/30 22:50:58 by jkosaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	cd_error(char *path_name)
{
	ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
	perror(path_name);
}

static void	no_current_dir(void)
{
	ft_putstr_fd("minishell: cd: error retrieving current directory:", \
													STDERR_FILENO);
	ft_putstr_fd("getcwd: cannot access parent directories:", STDERR_FILENO);
	ft_putendl_fd("No such file or directory", STDERR_FILENO);
}

static int	fail_chdir(char *path_name, t_sh_var *sh_var)
{
	char	*cwd_path;
	char	*relative_path;

	if (path_name[0] != '/')
	{
		cwd_path = getcwd(NULL, 0);
		if (cwd_path == NULL)
		{
			no_current_dir();
			free(sh_var->oldpwd);
			sh_var->oldpwd = sh_var->pwd;
			relative_path = ft_xstrjoin("/", path_name);
			sh_var->pwd = ft_xstrjoin(sh_var->pwd, relative_path);
			free(path_name);
			free(relative_path);
			set_env_value("PWD", sh_var->pwd, sh_var);
			set_env_value("OLDPWD", sh_var->oldpwd, sh_var);
			return (0);
		}
		free(cwd_path);
	}
	cd_error(path_name);
	free(path_name);
	return (1);
}

static int	builtin_cd_core(char *path_name, t_sh_var *sh_var)
{
	char	*dst_path;

	dst_path = cd_dst_path(path_name, sh_var);
	if (chdir(dst_path) == -1)
	{
		free(dst_path);
		return (fail_chdir(path_name, sh_var));
	}
	else
	{
		free(sh_var->oldpwd);
		sh_var->oldpwd = sh_var->pwd;
		sh_var->pwd = dst_path;
		set_env_value("PWD", sh_var->pwd, sh_var);
		set_env_value("OLDPWD", sh_var->oldpwd, sh_var);
		free(path_name);
		return (0);
	}
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
	return (builtin_cd_core(path_name, sh_var));
}
