#include "lexer.h"
#include "parser.h"

t_node	*new_node(t_node_kind kind, t_node *lhs, t_node *rhs)
{
	t_node	*node;

	node = malloc(sizeof(t_node));
	node->kind = kind;
	node->lhs = lhs;
	node->rhs = rhs;
	node->token = NULL;
	return (node);
}

// 比較と，一致していれば進める
bool	consume(char *op, t_token **cur_token)
{
	if (strcmp((*cur_token)->str, op) == 0)
	{
		*cur_token = (*cur_token)->next;
		return (true);
	}
	return (false);
}

//次の|か;か一番最後までを塊として読む
t_node	*create_process_node(t_token **cur_token)
{
	t_node	*node;
	t_token	*token_tail;

	node = malloc(sizeof(t_node));
	node->kind = ND_COMMAND;
	node->lhs = NULL;
	node->rhs = NULL;
	node->token = *cur_token;
	while ((*cur_token)->next->kind != TK_RESERVED && (*cur_token)->next->kind != TK_EOF)
	{
		if ((*cur_token)->kind == TK_REDIRECT && \
				(*cur_token)->next->kind == TK_REDIRECT)
			exit(1); // error_handler();
		*cur_token = (*cur_token)->next;
	} // < << を飛ばす
	token_tail = *cur_token;
	*cur_token = token_tail->next;
	token_tail->next = NULL;
	return (node);
}

t_token	**expect_process(t_token **cur_token)
{
	if ((*cur_token)->kind != TK_STRING)
	{
		printf("exit: not process / kind = %d\n", (*cur_token)->kind);
		exit(1); // error_handler();
	}
	return (cur_token);
}

// semicolon間の部分木
t_node	*expression(t_token **cur_token)
{
	t_node	*node;

	if ((*cur_token)->kind == TK_EOF)
		return (NULL);
	node = create_process_node(expect_process(cur_token));
	while (true)
	{
		if (consume("|", cur_token))
			node = new_node(ND_PIPE, node, create_process_node(expect_process(cur_token)));
		else
			return (node);
	}
}

// 全体のrootのnodeへのポインタを返す
t_node	*parser_sub(t_token *cur_token)
{
	t_node	*node;

	node = expression(&cur_token);
	while (true)
	{
		if (consume(";", &cur_token))
			node = new_node(ND_SEMICOLON, node, expression(&cur_token));
		else if (consume("&&", &cur_token))
			node = new_node(ND_DAND, node, expression(&cur_token));
		else if (consume("||", &cur_token))
			node = new_node(ND_DPIPE, node, expression(&cur_token));
		else
			return node;
	}
}

void	dfs(t_node *tree)
{
	if (tree->lhs)
		dfs(tree->lhs);
	printf("kind:%d\n", tree->kind);
	while(tree->token)
	{
		printf("%s\n", tree->token->str);
		tree->token = tree->token->next;
	}
	if (tree->rhs)
		dfs(tree->rhs);
}

t_node	*parser(char *argv)
{
	t_token	*tokens;
	t_node	*tree;

	// (void)argc;
	tokens = lexer(argv);
	tree = parser_sub(tokens);
	// dfs(tree);

	return (tree);
}

// int	main()
// {
// 	// parser("ls -al ; ; cat");
// 	// return (0);
// 	lexer("ls -al 2 &>1");
// 	printf("ok\n");
// 	return (0);
// }
