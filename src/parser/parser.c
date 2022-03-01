#include "lexer.h"
#include "parser.h"

t_node	*node_new(t_node_kind kind, t_node *lhs, t_node *rhs)
{
	t_node	*node;

	node = ft_calloc(1, sizeof(t_node));
	node->kind = kind;
	node->lhs = lhs;
	node->rhs = rhs;
	return (node);
}

// 比較と，一致していれば進める
bool	consume(char *op, t_list **cur_token_list)
{
	if (*cur_token_list == NULL)
		return (false);
	if (strcmp(((t_token *)((*cur_token_list)->content))->str, op) == 0)
	{
		*cur_token_list = (*cur_token_list)->next;
		return (true);
	}
	return (false);
}

//次の|か;か一番最後までを塊として読む
t_node	*create_process_node(t_list **cur_token_list)
{
	t_node	*node;
	t_list	*token_list_tail;

	node = ft_calloc(1, sizeof(t_node));
	node->kind = ND_PROCESS;
	node->token_list = *cur_token_list;
	while ((*cur_token_list)->next != NULL && \
		((t_token *)(((*cur_token_list)->next)->content))->kind != TK_RESERVED)
	{
		if (((t_token *)(*cur_token_list)->content)->kind == TK_REDIRECT && \
				((t_token *)(((*cur_token_list)->next)->content))->kind == TK_REDIRECT)
			exit(1); // error_handler(); // < や << などが連続したらエラー
		*cur_token_list = (*cur_token_list)->next;
	}
	if (((t_token *)(*cur_token_list)->content)->kind == TK_REDIRECT)
		exit(1); // error_handler(); // TK_REDIRECTが一番最後にあるとエラー
	token_list_tail = *cur_token_list;
	*cur_token_list = token_list_tail->next;
	token_list_tail->next = NULL;
	return (node);
}

t_list	**expect_process(t_list **cur_token_list)
{
	if (((t_token *)((*cur_token_list)->content))->kind != TK_STRING)
	{
		printf("exit: not process / kind = %d\n", \
				((t_token *)((*cur_token_list)->content))->kind);
		exit(1); // error_handler();
	}
	return (cur_token_list);
}

// semicolon間の部分木
t_node	*expression(t_list **cur_token_list)
{
	t_node	*node;

	if (*cur_token_list == NULL)
		return (NULL);
	node = create_process_node(expect_process(cur_token_list));
	while (true)
	{
		if (consume("|", cur_token_list))
			node = node_new(ND_PIPE, node, create_process_node(expect_process(cur_token_list)));
		else
			return (node);
	}
}

// 全体のrootのnodeへのポインタを返す
t_node	*parser_sub(t_list *cur_token_list)
{
	t_node	*node;

	node = expression(&cur_token_list);
	while (true)
	{
		if (consume(";", &cur_token_list))
			node = node_new(ND_SEMICOLON, node, expression(&cur_token_list));
		else if (consume("&&", &cur_token_list))
			node = node_new(ND_DAND, node, expression(&cur_token_list));
		else if (consume("||", &cur_token_list))
			node = node_new(ND_DPIPE, node, expression(&cur_token_list));
		else
			return node;
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

t_node	*parser(char *argv)
{
	t_list	*token_list;
	t_node	*tree;

	token_list = lexer(argv);
	tree = parser_sub(token_list);
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
