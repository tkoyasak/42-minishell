/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoyasak <tkoyasak@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 14:03:02 by jkosaka           #+#    #+#             */
/*   Updated: 2022/04/07 14:44:07 by tkoyasak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_pwd(t_proc *proc, t_sh_var *sh_var)
{
	(void)proc;
	printf("%s\n", sh_var->pwd);
	return (fd_error_handler("pwd"));
}
