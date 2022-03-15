/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_single_process.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkosaka <jkosaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/12 09:02:46 by jkosaka           #+#    #+#             */
/*   Updated: 2022/03/15 22:54:30 by jkosaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_single_external(t_expression *expression, t_process *process, t_shell_var *shell_var)
{
	int			wstatus;
	t_list		*process_list;

	process_list = expression->process_list;
	// set_redirections_and_commands(expression, shell_var);
	process = process_list->content;
	expression->pid[0] = fork();
	if (expression->pid[0] == 0)
		exec_child(expression, process, 0, shell_var);
	process_list = process_list->next;
	waitpid(expression->pid[0], &wstatus, WUNTRACED);
	return (WEXITSTATUS(wstatus));
}

int	exec_single_process(t_expression *expression, t_shell_var *shell_var)
{
	t_list		*process_list;
	t_process	*process;

	set_redirections_and_commands(expression, shell_var);
	process_list = expression->process_list;
	process = process_list->content;
	dup2_func(expression, process, 0);
	close_func(expression, process, 0);
	if (is_builtin(process->command[0]))
		return (exec_builtin(expression, process, shell_var));
	else
		return (exec_single_external(expression, process, shell_var));
}
