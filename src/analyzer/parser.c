/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoyasak <tkoyasak@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/25 10:48:38 by tkoyasak          #+#    #+#             */
/*   Updated: 2022/03/25 10:48:41 by tkoyasak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_node	*create_astree(t_list **itr, bool *is_valid);

static t_node	*create_subshell_tree(t_list **itr, bool *is_valid)
{
	t_node	*node;

	if (consume_node_kind(itr, "("))
	{
		node = ft_xcalloc(1, sizeof(t_node));
		node->kind = ND_SUBSHELL;
		node->lhs = create_astree(itr, is_valid);
		if (consume_node_kind(itr, ")") == false)
		{
			parser_error(itr, "(", is_valid, __LINE__);
			return (node);
		}
		return (node);
	}
	if (*itr == NULL)
		return (NULL);
	else if (((t_token *)((*itr)->content))->kind == TK_STRING || \
		((t_token *)((*itr)->content))->kind == TK_IO)
		return (create_proc_node(itr, is_valid));
	else if (((t_token *)((*itr)->content))->kind == TK_DELIM)
		return (parser_error(itr, ((t_token *)((*itr)->content))->str, \
			is_valid, __LINE__));
	else
		return (NULL);
}

// semicolon間の部分木
static t_node	*create_sub_astree(t_list **itr, bool *is_valid)
{
	t_node	*node;
	t_node	*r_node;

	if (*itr == NULL)
		return (NULL);
	node = create_subshell_tree(itr, is_valid);
	while (*is_valid)
	{
		if (consume_node_kind(itr, "|"))
		{
			r_node = create_subshell_tree(itr, is_valid);
			if (r_node == NULL)
				return (parser_error(itr, "|", is_valid, __LINE__));
			node = node_new(ND_PIPE, node, r_node);
		}
		else
			return (node);
	}
	return (node);
}

// 全体のrootのnodeへのポインタを返す
static t_node	*create_astree(t_list **itr, bool *is_valid)
{
	t_node		*node;

	if (*itr == NULL)
		return (NULL);
	node = create_sub_astree(itr, is_valid);
	while (*is_valid)
	{
		if (consume_node_kind(itr, ";"))
			node = node_new(ND_SEMICOLON, node, \
				create_sub_astree(itr, is_valid));
		else if (consume_node_kind(itr, "&&"))
			node = node_new(ND_DAND, node, create_sub_astree(itr, is_valid));
		else if (consume_node_kind(itr, "||"))
			node = node_new(ND_DPIPE, node, create_sub_astree(itr, is_valid));
		else
			return (node);
	}
	return (node);
}

/*  token_listからND_SEMICOLON, ND_PIPE, ND_PROCの
nodeで形成されるtreeのrootを返す  */
int	parser(t_node **tree, t_list *token_list)
{
	bool	is_valid;

	is_valid = true;
	*tree = create_astree(&token_list, &is_valid);
	return (!is_valid);
}
