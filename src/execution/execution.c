#include "minishell.h"

/*  initialize expr. prepare pipefd and pid  */
// pipe_fdはproc_cnt-1個用意
void	init_expr(t_expr *expr)
{
	int	pipe_cnt;

	pipe_cnt = expr->proc_cnt - 1;
	expr->pipefd = (int **)ft_xcalloc(pipe_cnt, sizeof(int *));
	expr->pid = \
			(pid_t *)ft_xcalloc(expr->proc_cnt, sizeof(pid_t));
}

/*  expr is between semicolon, double ampersand, and double pipe  */
int	evaluate_expr(t_expr *expr, t_shell_var *shell_var)
{
	int		stdin_copy;
	int		stdout_copy;

	init_expr(expr);
	expansion(expr, shell_var);
	if (((t_proc *)(expr->proc_list->content))->token_list == NULL)
		;
	else if (expr->proc_cnt == 1)
	{
		stdin_copy = safe_func(dup(STDIN_FILENO));
		stdout_copy = safe_func(dup(STDOUT_FILENO));
		g_exit_status = exec_single_proc(expr, shell_var);
		safe_func(dup2(stdin_copy, STDIN_FILENO));
		safe_func(dup2(stdout_copy, STDOUT_FILENO));
		safe_func(close(stdin_copy));
		safe_func(close(stdout_copy));
	}
	else
		g_exit_status = exec_procs(expr, shell_var);
	return (g_exit_status);
}

/*  execute subshell. does not affect outside  */
void	exec_subshell(t_node *tree, t_shell_var *shell_var)
{
	int				wstatus;
	pid_t			pid;

	pid = safe_func(fork());
	if (pid == 0)
	{
		g_exit_status = execution(tree->lhs, shell_var);
		exit(g_exit_status);
	}
	else
	{
		safe_func(waitpid(pid, &wstatus, WUNTRACED));
		g_exit_status = wstatus;
	}
}

/*  evaluate expr of tree, or ececute lhs of tree ans rhs of tree  */
int	execution(t_node *tree, t_shell_var *shell_var)
{
	if (tree->kind == ND_SUBSHELL)
		exec_subshell(tree, shell_var);
	else if (ND_SEMICOLON <= tree->kind && tree->kind <= ND_DPIPE)
	{
		if (tree->lhs)
			g_exit_status = execution(tree->lhs, shell_var);
		if (tree->kind == ND_DAND && g_exit_status != 0)
			return (g_exit_status);
		if (tree->kind == ND_DPIPE && g_exit_status == 0)
			return (g_exit_status);
		if (tree->rhs)
			g_exit_status = execution(tree->rhs, shell_var);
	}
	else
		g_exit_status = evaluate_expr(tree->expr, shell_var);
	return (g_exit_status);
}

// int	main(void)
// {
// 	// t_node *tree = expansion("cat < infile | cat < infile2 | 'hello'; $SHELL && echo hey! > outfile");
// 	// t_node *tree = expansion("ls -a | cat");
// 	// t_node *tree = expansion("cat < infile | cat > outfile");
// 	// t_list *expr_list = convert_to_expr_list(tree);
// 	execution(tree);

// 	printf("==============\n");
// 	while (expr_list)
// 	{
// 		t_list *proc_list = ((t_expr *)(expr_list->content))->proc_list;
// 		static int cnt = 0; cnt++;
// 		printf("%d %d\n", cnt, ((t_expr *)(expr_list->content))->end_of_expr);
// 		while (proc_list)
// 		{
// 			t_list *itr = ((t_proc *)(proc_list->content))->token_list;
// 			while (itr)
// 			{
// 				printf("%s ", ((t_token *)(itr->content))->str);
// 				itr = itr->next;
// 			}
// 			proc_list = proc_list->next;
// 			printf("\n");
// 		}
// 		expr_list = expr_list->next;
// 	}
// }
