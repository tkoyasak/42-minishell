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
static int	evaluate_expr(t_expr *expr, t_sh_var *sh_var)
{
	int		stdin_copy;
	int		stdout_copy;

	init_expr(expr);
	expander(expr, sh_var);
	if (((t_proc *)(expr->proc_list->content))->token_list == NULL)
		;
	else if (expr->proc_cnt == 1)
	{
		stdin_copy = safe_func(dup(STDIN_FILENO));
		stdout_copy = safe_func(dup(STDOUT_FILENO));
		g_exit_status = exec_single_proc(expr, sh_var);
		safe_func(dup2(stdin_copy, STDIN_FILENO));
		safe_func(dup2(stdout_copy, STDOUT_FILENO));
		safe_func(close(stdin_copy));
		safe_func(close(stdout_copy));
	}
	else
		g_exit_status = exec_procs(expr, sh_var);
	return (g_exit_status);
}

/*  execute subshell. does not affect outside job  */
static void	exec_subshell(t_node *tree, t_sh_var *sh_var)
{
	int				wstatus;
	pid_t			pid;

	pid = safe_func(fork());
	if (pid == 0)
	{
		g_exit_status = executor(tree->lhs, sh_var);
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
int	executor(t_node *tree, t_sh_var *sh_var)
{
	if (tree->kind == ND_SUBSHELL)
		exec_subshell(tree, sh_var);
	else if (ND_SEMICOLON <= tree->kind && tree->kind <= ND_DPIPE)
	{
		if (tree->lhs)
			g_exit_status = executor(tree->lhs, sh_var);
		if (tree->kind == ND_DAND && g_exit_status != 0)
			return (g_exit_status);
		if (tree->kind == ND_DPIPE && g_exit_status == 0)
			return (g_exit_status);
		if (tree->rhs)
			g_exit_status = executor(tree->rhs, sh_var);
	}
	else
		g_exit_status = evaluate_expr(tree->expr, sh_var);
	return (g_exit_status);
}
