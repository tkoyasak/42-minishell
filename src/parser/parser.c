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

// semicolon間の部分木
t_node	*expression(t_token **cur_token)
{
	t_node	*node;

	node = create_process_node(cur_token);
	while (true)
	{
		printf("59:%s\n", (*cur_token)->str);
		if (consume("|", cur_token))
			node = new_node(ND_PIPE, node, create_process_node(cur_token));
		else
		{
			printf("61:%s\n", (*cur_token)->str);
			return (node);
		}
	}
}

// 全体のrootのnodeへのポインタを返す
t_node	*parser(t_token *cur_token)
{
	t_node	*node;

	node = expression(&cur_token);
	while (true)
	{
		printf("74:%s\n", cur_token->str);
		if (consume(";", &cur_token))
			node = new_node(ND_SEMICOLON, node, expression(&cur_token));
		else
			return node;
	}
}

int	main(int argc, char **argv)
{
	t_token	*tokens;
	t_node	*tree;

	(void)argc;
	tokens = lexer(argv[1]);
	tree = parser(tokens);
	(void)tree;

	return (0);
}
