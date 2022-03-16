#include "minishell.h"

static t_node	*create_astree(t_list **itr);

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
static t_node	*create_process_node(t_list **itr)
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
			exit(1); // error_handler(); // < や << などが連続したらエラー
		*itr = (*itr)->next;
	}
	if (((t_token *)(*itr)->content)->kind == TK_REDIRECT)
		exit(1); // error_handler(); // TK_REDIRECTが一番最後にあるとエラー
	tail = *itr;
	*itr = tail->next;
	tail->next = NULL;
	return (node);
}

static t_list	**expect_process(t_list **itr)
{
	if (((t_token *)((*itr)->content))->kind != TK_STRING)
	{
		ft_putstr_fd("syntax error near unexpected token `", STDERR_FILENO);
		ft_putstr_fd(((t_token *)((*itr)->content))->str, STDERR_FILENO);
		ft_putstr_fd("'\n", STDERR_FILENO);
		exit(1); // error_handler();
	}
	return (itr);
}

static t_node	*create_subshell_tree(t_list **itr)
{
	t_node	*node;

	if (consume_node_kind(itr, "("))
	{
		node = ft_calloc(1, sizeof(t_node));
		node->kind = ND_SUBSHELL;
		// printf("%s\n", ((t_token *)((*itr)->content))->str);
		node->lhs = create_astree(itr);
		// printf("%s\n", ((t_token *)((*itr)->content))->str);
		if (consume_node_kind(itr, ")") == false)
		{
			ft_putstr_fd("syntax error near unexpected token `", STDERR_FILENO);
			ft_putstr_fd(((t_token *)((*itr)->content))->str, STDERR_FILENO);
			ft_putstr_fd("'\n", STDERR_FILENO);
			exit(1); // error_handler();
		}
		return (node);
	}
	return (create_process_node(expect_process(itr)));
}

// semicolon間の部分木
static t_node	*create_sub_astree(t_list **itr)
{
	t_node	*node;

	if (*itr == NULL)
		return (NULL);
	node = create_subshell_tree(itr);
	while (true)
	{
		if (consume_node_kind(itr, "|"))
			node = node_new(ND_PIPE, node, create_subshell_tree(itr));
		else
			return (node);
	}
}

// 全体のrootのnodeへのポインタを返す
static t_node	*create_astree(t_list **itr)
{
	t_node		*node;

	node = create_sub_astree(itr);
	while (true)
	{
		if (consume_node_kind(itr, ";"))
			node = node_new(ND_SEMICOLON, node, create_sub_astree(itr));
		else if (consume_node_kind(itr, "&&"))
			node = node_new(ND_DAND, node, create_sub_astree(itr));
		else if (consume_node_kind(itr, "||"))
			node = node_new(ND_DPIPE, node, create_sub_astree(itr));
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
t_node	*parser(char *argv)
{
	t_list	*token_list;
	t_node	*tree;

	token_list = lexer(argv);
	tree = create_astree(&token_list);
	// dfs(tree);
	return (tree);
}

// int	main()
// {
// 	// parser("ls -al ; ; cat");
// 	// return (0);
// 	parser("ls -al | cat $PATH");
// 	printf("ok\n");
// 	return (0);
// }
