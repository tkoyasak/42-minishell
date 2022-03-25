/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delete_content.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoyasak <tkoyasak@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/25 11:05:57 by tkoyasak          #+#    #+#             */
/*   Updated: 2022/03/25 12:02:50 by tkoyasak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	delete_env(void *arg)
{
	t_env	*env;

	if (!arg)
		return ;
	env = (t_env *)arg;
	free(env->key);
	free(env->val);
	free(env);
	env = NULL;
}

void	delete_token(void *arg)
{
	t_token	*token;

	if (!arg)
		return ;
	token = (t_token *)arg;
	free(token->str);
	free(token);
	token = NULL;
}

void	delete_expd(void *arg)
{
	t_expd	*expd;

	if (!arg)
		return ;
	expd = (t_expd *)arg;
	free(expd->str);
	free(expd);
	expd = NULL;
}

void	delete_proc(void *arg)
{
	t_proc	*proc;

	if (!arg)
		return ;
	proc = (t_proc *)arg;
	ft_lstclear(&proc->token_list, delete_token);
	free(proc->filename[0]);
	free(proc->filename[1]);
	free(proc->command);
	free(proc);
	proc = NULL;
}

void	delete_expr(void *arg)
{
	t_expr	*expr;
	int		i;

	if (!arg)
		return ;
	expr = (t_expr *)arg;
	ft_lstclear(&expr->proc_list, delete_proc);
	if (expr->pipefd)
	{
		i = -1;
		while (++i < expr->proc_cnt - 1)
			free(expr->pipefd[i]);
	}
	free(expr->pipefd);
	free(expr->pid);
	free(expr);
	expr = NULL;
}
