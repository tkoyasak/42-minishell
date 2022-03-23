#include "minishell.h"

void	delete_env(void *arg)
{
	t_env	*env;

	env = (t_env *)arg;
	free(env->key);
	free(env->val);
	free(env);
	env = NULL;
}

void	delete_token(void *arg)
{
	t_token	*token;

	token = (t_token *)arg;
	free(token->str);
	free(token);
	token = NULL;
}

void	delete_expansion(void *arg)
{
	t_expansion	*expansion;

	expansion = (t_expansion *)arg;
	free(expansion->str);
	free(expansion);
	expansion = NULL;
}

void	delete_process(void *arg)
{
	t_process	*process;
	int			i;

	process = (t_process *)arg;
	ft_lstclear(&process->token_list, delete_token);
	if (process->fd[0] > 2)
		safe_func(close(process->fd[0]));
	if (process->fd[1] > 2)
		safe_func(close(process->fd[1]));
	if (process->here_pipefd[0] > 2)
		safe_func(close(process->here_pipefd[0]));
	if (process->here_pipefd[1] > 2)
		safe_func(close(process->here_pipefd[1]));
	free(process->filename[0]);
	free(process->filename[1]);
	i = -1;
	while (process->command[++i])
		free(process->command[i]);
	free(process->command);
	free(process);
	process = NULL;
}

void	delete_expr(void *arg)
{
	t_expr	*expr;
	int				i;

	expr = (t_expr *)arg;
	ft_lstclear(&expr->process_list, delete_process);
	i = -1;
	while (++i < expr->process_cnt - 1)
		free(expr->pipefd[i]);
	free(expr->pipefd);
	free(expr);
	expr = NULL;
}

void	delete_node(void *arg)
{
	t_node	*node;

	node = (t_node *)arg;
	ft_lstclear(&node->token_list, delete_token);
	delete_expr(node->expr);
	free(node);
	node = NULL;
}

void	delete_astree(t_node *node)
{
	if (node->lhs)
		delete_astree(node->lhs);
	if (node->rhs)
		delete_astree(node->rhs);
	delete_node(node);
}
