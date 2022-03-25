/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkosaka <jkosaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 13:57:29 by jkosaka           #+#    #+#             */
/*   Updated: 2022/03/25 17:20:18 by jkosaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_cd_pwd_update(char *path_name, t_sh_var *sh_var)
{
	char	*relative_path;
	char	*cwd_path;

	free(sh_var->oldpwd);
	sh_var->oldpwd = sh_var->pwd;
	cwd_path = getcwd(NULL, 0);
	if (cwd_path == NULL)
	{
		ft_putendl_fd("cd: error retrieving current directory: \
			getcwd: cannot access parent directories: \
			No such file or directory", STDERR_FILENO);
		relative_path = ft_xstrjoin("/", path_name);
		sh_var->pwd = ft_xstrjoin_free(sh_var->pwd, relative_path, true);
	}
	else
		sh_var->pwd = cwd_path;
	free(path_name);
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
		{
			detect_leak(__LINE__, __FILE__);
			free(path_name);
			return (0);
		}
	}
	if (chdir(path_name) == -1)
	{
		ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
		ft_putstr_fd(path_name, STDERR_FILENO);
		ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
		free(path_name);
		return (1);
	}
	return (builtin_cd_pwd_update(path_name, sh_var));
}
