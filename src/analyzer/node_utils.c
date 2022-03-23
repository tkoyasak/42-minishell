#include "minishell.h"

t_node	*parser_error(t_list **itr, char *str, int *parser_result, int line)
{
	(void)line;
	if (*parser_result != 1)
	{
		*parser_result = 1;
		// printf("syntax:%d\n", line);
		ft_putstr_fd("minishell: syntax error near unexpected token `", STDERR_FILENO);
		ft_putstr_fd(str, STDERR_FILENO);
		ft_putstr_fd("'\n", STDERR_FILENO);
	}
	ft_lstclear(itr, delete_token);
	return (NULL);
}

t_node	*node_new(t_node_kind kind, t_node *lhs, t_node *rhs)
{
	t_node	*node;

	node = ft_xcalloc(1, sizeof(t_node));
	node->kind = kind;
	node->lhs = lhs;
	node->rhs = rhs;
	return (node);
}

// 比較と，一致していれば進める
bool	consume_node_kind(t_list **itr, char *op)
{
	t_list	*delete_itr;

	if (*itr && ft_strcmp(((t_token *)((*itr)->content))->str, op) == 0)
	{
		delete_itr = *itr;
		*itr = (*itr)->next;
		ft_lstdelone(delete_itr, delete_token); // 元のトークンはいらなくなるので，deleteが必要．
		return (true);
	}
	return (false);
}

// 次の|か;か一番最後までを塊として読む
t_node	*create_process_node(t_list **itr, int *parser_result)
{
	t_node	*node;
	t_list	*tail;

	node = ft_xcalloc(1, sizeof(t_node));
	node->kind = ND_PROCESS;
	node->token_list = *itr;
	while ((*itr)->next != NULL && \
		((t_token *)((*itr)->next->content))->kind != TK_PROCESS_DELIM && \
		((t_token *)((*itr)->next->content))->kind != TK_L_PARENTHESIS && \
		((t_token *)((*itr)->next->content))->kind != TK_R_PARENTHESIS)
	{
		if (((t_token *)(*itr)->content)->kind == TK_REDIRECT && \
				((t_token *)((*itr)->next->content))->kind != TK_STRING)
		{
			*parser_result = 1;
			return (node);
			// ここでNULLが返ると，tokenをフリーできなくなる．
		}
		*itr = (*itr)->next;
	}
	tail = *itr;
	*itr = tail->next;
	tail->next = NULL;
	if (((t_token *)tail->content)->kind == TK_REDIRECT || \
		(*itr && ((t_token *)(*itr)->content)->kind == TK_L_PARENTHESIS))
	{
		parser_error(itr, ((t_token *)tail->content)->str, parser_result, __LINE__);
		return (node);
	}
	return (node);
}
