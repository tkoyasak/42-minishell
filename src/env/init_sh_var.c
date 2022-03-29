/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_sh_var.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoyasak <tkoyasak@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/29 12:46:50 by tkoyasak          #+#    #+#             */
/*   Updated: 2022/03/30 00:16:47 by tkoyasak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_same_dir(char *dir1, char *dir2)
{
	struct stat		s1;
	struct stat		s2;

	if (!dir1 || !dir2)
		return (false);
	if (stat(dir1, &s1) == -1 || stat(dir2, &s2) == -1)
		return (false);
	if (s1.st_dev == s2.st_dev && s1.st_ino == s2.st_ino)
		return (true);
	return (false);
}

static void	init_pwd(t_sh_var *sh_var)
{
	char	*pwd_val;
	char	*cwd_val;

	pwd_val = get_env_value("PWD", sh_var);
	if (!pwd_val)
		set_env_value("PWD", NULL, sh_var);
	cwd_val = getcwd(NULL, 0);
	if (!cwd_val)
		error_handler("getcwd");
	if (!pwd_val || !is_same_dir(pwd_val, cwd_val))
	{
		sh_var->pwd = cwd_val;
		free(pwd_val);
	}
	else
	{
		sh_var->pwd = pwd_val;
		free(cwd_val);
	}
	set_env_value("PWD", sh_var->pwd, sh_var);
}

static void	init_old_pwd(t_sh_var *sh_var)
{
	sh_var->oldpwd = NULL;
	set_env_value("OLDPWD", sh_var->oldpwd, sh_var);
}

void	init_sh_var(t_sh_var *sh_var)
{
	sh_var->env_list = init_envlist();
	init_pwd(sh_var);
	init_old_pwd(sh_var);
}
