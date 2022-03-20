#include "minishell.h"

static t_node	*create_astree(t_list **itr, int *parser_result);

t_node	*parser_error_handler(t_list **itr, char *str, int *parser_result, int line)
{
	if (*parser_result != 1)
	{
		*parser_result = 1;
		printf("syntax:%d\n", line);
		ft_putstr_fd("syntax error near unexpected token `", STDERR_FILENO);
		ft_putstr_fd(str, STDERR_FILENO);
		ft_putstr_fd("'\n", STDERR_FILENO);
	}
	ft_lstclear(itr, delete_token);
	return (NULL);
}

static t_node	*node_new(t_node_kind kind, t_node *lhs, t_node *rhs)
{
	t_node	*node;

	node = ft_calloc(1, sizeof(t_node));
	node->kind = kind;
	node->lhs = lhs;
	node->rhs = rhs;
	return (node);
}

// 比較と，一致していれば進める
static bool	consume_node_kind(t_list **itr, char *op)
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
static t_node	*create_process_node(t_list **itr, int *parser_result)
{
	t_node	*node;
	t_list	*tail;

	node = ft_calloc(1, sizeof(t_node));
	node->kind = ND_PROCESS;
	node->token_list = *itr;
	while ((*itr)->next != NULL && \
		((t_token *)((*itr)->next->content))->kind != TK_PROCESS_DELIM && \
		((t_token *)((*itr)->next->content))->kind != TK_L_PARENTHESIS && \
		((t_token *)((*itr)->next->content))->kind != TK_R_PARENTHESIS)
	{
		if (((t_token *)(*itr)->content)->kind == TK_REDIRECT && \
				((t_token *)((*itr)->next->content))->kind == TK_REDIRECT)
		{
			parser_error_handler(itr, ((t_token *)((*itr)->next->content))->str, parser_result, __LINE__);
			return (node);
			// return (parser_error_handler(itr, ((t_token *)(*itr)->content)->str, parser_result, __LINE__));
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
		parser_error_handler(itr, ((t_token *)tail->content)->str, parser_result, __LINE__);
		return (node);
	}
	return (node);
}

static void	expect_process(t_list **itr, int *parser_result)
{
	if (((t_token *)((*itr)->content))->kind != TK_STRING)
	{
		parser_error_handler(itr, ((t_token *)((*itr)->content))->str, parser_result, __LINE__);
	}
}

static t_node	*create_subshell_tree(t_list **itr, int *parser_result)
{
	t_node	*node;

	if (consume_node_kind(itr, "("))
	{
		node = ft_calloc(1, sizeof(t_node));
		node->kind = ND_SUBSHELL;
		node->lhs = create_astree(itr, parser_result);
		if (consume_node_kind(itr, ")") == false)
		{
			// printf("95\n");
			parser_error_handler(itr, "(", parser_result, __LINE__);
			return (node);
		}
		return (node);
	}
	if (*itr == NULL)
		return (NULL);
	else if (((t_token *)((*itr)->content))->kind == TK_STRING)
		return (create_process_node(itr, parser_result));
	else if (((t_token *)((*itr)->content))->kind == TK_PROCESS_DELIM)
		return (parser_error_handler(itr, ((t_token *)((*itr)->content))->str, parser_result, __LINE__));
	else
		return (NULL);
}

// semicolon間の部分木
static t_node	*create_sub_astree(t_list **itr, int *parser_result)
{
	t_node	*node;
	t_node	*r_node;

	if (*itr == NULL)
		return (NULL);
	node = create_subshell_tree(itr, parser_result);
	while (*parser_result == 0)
	{
		if (consume_node_kind(itr, "|"))
		{
			r_node = create_subshell_tree(itr, parser_result);
			if (r_node == NULL)
				return (parser_error_handler(itr, "|", parser_result, __LINE__));
			node = node_new(ND_PIPE, node, r_node);
		}
		else
			return (node);
	}
	return (node);
}

// 全体のrootのnodeへのポインタを返す
static t_node	*create_astree(t_list **itr, int *parser_result)
{
	t_node		*node;

	if (*itr == NULL)
		return (NULL);
	node = create_sub_astree(itr, parser_result);
	while (*parser_result == 0)
	{
		if (consume_node_kind(itr, ";"))
			node = node_new(ND_SEMICOLON, node, create_sub_astree(itr, parser_result));
		else if (consume_node_kind(itr, "&&"))
			node = node_new(ND_DAND, node, create_sub_astree(itr, parser_result));
		else if (consume_node_kind(itr, "||"))
			node = node_new(ND_DPIPE, node, create_sub_astree(itr, parser_result));
		else
			return (node);
	}
	return (node);
}

// static void	dfs(t_node *tree)
// {
// 	if (tree->lhs)
// 		dfs(tree->lhs);
// 	printf("kind:%d\n", tree->kind);
// 	while (tree->token)
// 	{
// 		printf("%s\n", tree->token->str);
// 		tree->token = tree->token->next;
// 	}
// 	if (tree->rhs)
// 		dfs(tree->rhs);
// }

/*  token_listからND_SEMICOLON, ND_PIPE, ND_PROCESSの
nodeで形成されるtreeのrootを返す  */
int	parser(t_node **tree, t_list *token_list)
{
	int	parser_result;

	parser_result = 0;
	*tree = create_astree(&token_list, &parser_result);
	// printf("168: %d\n", parser_result);
	// dfs(tree);
	// printf("parser_result: %d\n", parser_result);
	return (parser_result);
}

// int	main()
// {
// 	// parser("ls -al ; ; cat");
// 	// return (0);
// 	parser("ls -al | cat $PATH");
// 	printf("ok\n");
// 	return (0);
// }
