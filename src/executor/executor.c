/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoyasak <tkoyasak@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 11:10:29 by jkosaka           #+#    #+#             */
/*   Updated: 2022/03/24 16:29:19 by tkoyasak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*  initialize expression. prepare pipefd and pid  */
static void	init_expr(t_expr *expr)
{
	int	pipe_cnt;

	pipe_cnt = expr->proc_cnt - 1;
	expr->pipefd = (int **)ft_xcalloc(pipe_cnt, sizeof(int *));
	expr->pid = (pid_t *)ft_xcalloc(expr->proc_cnt, sizeof(pid_t));
}

/*  expression is between semicolon, double ampersand, and double pipe  */
static void	evaluate_expr(t_expr *expr, t_sh_var *sh_var)
{
	t_list	*token_list;

	init_expr(expr);
	expander(expr, sh_var);
	token_list = ((t_proc *)(expr->proc_list->content))->token_list;
	if (token_list == NULL)
		;
	else if (expr->proc_cnt >= PROC_MAX)
	{
		print_error_msg("fork", "Resource temporarily unavailable");
		g_exit_status = 1;
	}
	else if (expr->proc_cnt == 1 && is_builtin(first_command(token_list)))
		g_exit_status = exec_single_builtin(expr, sh_var);
	else
		g_exit_status = exec_procs(expr, sh_var);
}

/*  execute subshell. does not affect outside job  */
static void	exec_subshell(t_node *tree, t_sh_var *sh_var)
{
	int				wstatus;
	pid_t			pid;

	pid = safe_func(fork());
	if (pid == 0)
	{
		executor(tree->lhs, sh_var);
		delete_astree(tree);
		exit(g_exit_status);
	}
	else
	{
		safe_func(waitpid(pid, &wstatus, WUNTRACED));
		g_exit_status = wstatus;
	}
}

/*  evaluate expression of tree, or execute lhs and rhs of tree  */
void	executor(t_node *tree, t_sh_var *sh_var)
{
	if (tree->kind == ND_SUBSHELL)
		exec_subshell(tree, sh_var);
	else if (ND_SEMICOLON <= tree->kind && tree->kind <= ND_DPIPE)
	{
		if (tree->lhs)
			executor(tree->lhs, sh_var);
		if (tree->kind == ND_DAND && g_exit_status != 0)
			return ;
		if (tree->kind == ND_DPIPE && g_exit_status == 0)
			return ;
		if (tree->rhs)
			executor(tree->rhs, sh_var);
	}
	else
		evaluate_expr(tree->expr, sh_var);
}
