#include "minishell.h"

static t_node	*create_astree(t_list **itr, int *parser_result);

t_node	*parser_error_handler(char *str, int *parser_result)
{
	*parser_result = 1;
	ft_putstr_fd("syntax error near unexpected token `", STDERR_FILENO);
	ft_putstr_fd(str, STDERR_FILENO);
	ft_putstr_fd("'\n", STDERR_FILENO);
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
	if (*itr == NULL)
		return (false);
	if (ft_strcmp(((t_token *)((*itr)->content))->str, op) == 0)
	{
		*itr = (*itr)->next;
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
			return (parser_error_handler(((t_token *)(*itr)->content)->str, parser_result));
		}
		*itr = (*itr)->next;
	}
	tail = *itr;
	*itr = tail->next;
	tail->next = NULL;
	if (((t_token *)tail->content)->kind == TK_REDIRECT || \
		(*itr && ((t_token *)(*itr)->content)->kind == TK_L_PARENTHESIS))
	{
		return (parser_error_handler(((t_token *)tail->content)->str, parser_result));
	}
	return (node);
}

static void	expect_process(t_list **itr, int *parser_result)
{
	if (((t_token *)((*itr)->content))->kind != TK_STRING)
	{
		parser_error_handler(((t_token *)((*itr)->content))->str, parser_result);
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
			return (parser_error_handler("(", parser_result));
		}
		return (node);
	}
	expect_process(itr, parser_result);
	if (*parser_result)
		return (NULL);
	return (create_process_node(itr, parser_result));
}

// semicolon間の部分木
static t_node	*create_sub_astree(t_list **itr, int *parser_result)
{
	t_node	*node;

	if (*itr == NULL)
		return (NULL);
	node = create_subshell_tree(itr, parser_result);
	while (true)
	{
		if (consume_node_kind(itr, "|"))
			node = node_new(ND_PIPE, node, create_subshell_tree(itr, parser_result));
		else
			return (node);
	}
}

// 全体のrootのnodeへのポインタを返す
static t_node	*create_astree(t_list **itr, int *parser_result)
{
	t_node		*node;

	node = create_sub_astree(itr, parser_result);
	while (true)
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
