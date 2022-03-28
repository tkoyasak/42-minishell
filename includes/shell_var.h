/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_var.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkosaka <jkosaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/25 11:05:10 by tkoyasak          #+#    #+#             */
/*   Updated: 2022/03/28 15:59:37 by jkosaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_VAR_H
# define SHELL_VAR_H

# include "libft.h"
# include "struct.h"

t_list		*init_envlist(void);
char		**get_environ(t_sh_var *sh_var);
char		*get_env_value(char *key, t_sh_var *sh_var);
void		set_env_value(char *key, char *val, t_sh_var *sh_var);
char		*get_env_value_str(char *key, t_sh_var *sh_var);
void		sort_env_list(t_list *lst);

#endif
