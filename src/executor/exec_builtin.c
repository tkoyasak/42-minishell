/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkosaka <jkosaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 11:11:26 by jkosaka           #+#    #+#             */
/*   Updated: 2022/04/08 16:32:31 by jkosaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*  retrun true if command is builtin  */
bool	is_builtin(char *cmd)
{
	const char	*builtin_str[] = \
			{"echo", "cd", "pwd", "export", "unset", "env", "exit"};
	int			idx;

	if (cmd == NULL)
		return (false);
	idx = 0;
	while (idx < 7)
	{
		if (ft_strcmp(cmd, builtin_str[idx]) == 0)
			return (true);
		idx++;
	}
	return (false);
}

/*  execute builtin. return exit status  */
int	exec_builtin(t_proc *proc, t_sh_var *sh_var)
{
	const char	*builtin_str[] = \
			{"echo", "cd", "pwd", "export", "unset", "env", "exit"};
	static	int	(*builtin_func[])(t_proc *, t_sh_var *) = \
			{&builtin_echo, &builtin_cd, &builtin_pwd, &builtin_export, \
			&builtin_unset, &builtin_env, &builtin_exit};
	char		*cmd;
	int			idx;

	cmd = proc->command[0];
	idx = 0;
	while (idx < 7)
	{
		if (ft_strcmp(cmd, builtin_str[idx]) == 0)
			return ((*builtin_func[idx])(proc, sh_var));
		idx++;
	}
	return (1);
}

char	*first_command(t_list *token_list)
{
	t_list	*itr;
	t_token	*token;

	itr = token_list;
	while (itr)
	{
		token = itr->content;
		if (token->kind == TK_IO)
			itr = itr->next->next;
		else
			return (token->str);
	}
	return (NULL);
}
