#include "minishell.h"

void	delete_env(void *arg)
{
	t_env	*env;

	env = (t_env *)arg;
	free(env->key);
	free(env->val);
	free(env);
}

void	delete_token(void *arg)
{
	t_token	*token;

	token = (t_token *)arg;
	free(token->str);
	free(token);
}

void	delete_expansion(void *arg)
{
	t_expansion	*expansion;

	expansion = (t_expansion *)arg;
	free(expansion->str);
	free(expansion);
}

void	delete_process(void *arg)
{
	t_process	*process;
	size_t		i;

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
	free(process->heredoc);
	i = -1;
	while (process->command[++i])
		free(process->command[i]);
	free(process->command);
	free(process);
}

void	delete_expression(void *arg)
{
	t_expression	*expression;
	size_t			i;

	expression = (t_expression *)arg;
	ft_lstclear(&expression->process_list, delete_process);
	i = -1;
	while (expression->pipefd[++i])
		free(expression->pipefd[i]);
	free(expression->pipefd);
	free(expression);
}

void	delete_node(void *arg)
{
	t_node	*node;

	node = (t_node *)arg;
	ft_lstclear(&node->token_list, delete_token);
	delete_expression(node->expression);
	free(node);
}

void	delete_astree(t_node *node)
{
	if (node->lhs)
		delete_astree(node->lhs);
	if (node->rhs)
		delete_astree(node->rhs);
	delete_node(node);
}
