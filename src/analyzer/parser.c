#include "minishell.h"

static t_node	*create_astree(t_list **itr, bool *parser_result);

static t_node	*create_subshell_tree(t_list **itr, bool *parser_result)
{
	t_node	*node;

	if (consume_node_kind(itr, "("))
	{
		node = ft_xcalloc(1, sizeof(t_node));
		node->kind = ND_SUBSHELL;
		node->lhs = create_astree(itr, parser_result);
		if (consume_node_kind(itr, ")") == false)
		{
			// printf("95\n");
			parser_error(itr, "(", parser_result, __LINE__);
			return (node);
		}
		return (node);
	}
	if (*itr == NULL)
		return (NULL);
	else if (((t_token *)((*itr)->content))->kind == TK_STRING || \
		((t_token *)((*itr)->content))->kind == TK_REDIRECT)
		return (create_process_node(itr, parser_result));
	else if (((t_token *)((*itr)->content))->kind == TK_PROCESS_DELIM)
		return (parser_error(itr, ((t_token *)((*itr)->content))->str, parser_result, __LINE__));
	else
		return (NULL);
}

// semicolon間の部分木
static t_node	*create_sub_astree(t_list **itr, bool *parser_result)
{
	t_node	*node;
	t_node	*r_node;

	if (*itr == NULL)
		return (NULL);
	node = create_subshell_tree(itr, parser_result);
	while (*parser_result)
	{
		if (consume_node_kind(itr, "|"))
		{
			r_node = create_subshell_tree(itr, parser_result);
			if (r_node == NULL)
				return (parser_error(itr, "|", parser_result, __LINE__));
			node = node_new(ND_PIPE, node, r_node);
		}
		else
			return (node);
	}
	return (node);
}

// 全体のrootのnodeへのポインタを返す
static t_node	*create_astree(t_list **itr, bool *parser_result)
{
	t_node		*node;

	if (*itr == NULL)
		return (NULL);
	node = create_sub_astree(itr, parser_result);
	while (*parser_result)
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
	bool	parser_result;

	parser_result = true; // is_valid
	*tree = create_astree(&token_list, &parser_result);
	// printf("168: %d\n", parser_result);
	// dfs(tree);
	// printf("parser_result: %d\n", parser_result);
	return (!parser_result);
}

// int	main()
// {
// 	// parser("ls -al ; ; cat");
// 	// return (0);
// 	parser("ls -al | cat $PATH");
// 	printf("ok\n");
// 	return (0);
// }
