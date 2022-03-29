/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoyasak <tkoyasak@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/25 10:48:38 by tkoyasak          #+#    #+#             */
/*   Updated: 2022/03/29 21:38:45 by tkoyasak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_node	*create_astree(t_list **itr, bool *is_valid);

static t_node	*create_subshell_tree(t_list **itr, bool *is_valid)
{
	t_node	*node;

	if (consume_node_kind(itr, "(", is_valid))
	{
		node = node_new(ND_SUBSHELL, create_astree(itr, is_valid), NULL);
		if (!consume_node_kind(itr, ")", is_valid))
		{
			parser_error(itr, "(", is_valid);
			return (node);
		}
		return (node);
	}
	return (create_proc_node(itr, is_valid));
}

static t_node	*create_sub_astree(t_list **itr, bool *is_valid)
{
	t_node	*node;
	t_node	*rhs;

	if (!*is_valid || !*itr)
		return (NULL);
	node = create_subshell_tree(itr, is_valid);
	while (*is_valid)
	{
		if (consume_node_kind(itr, "|", is_valid))
		{
			rhs = create_subshell_tree(itr, is_valid);
			if (rhs == NULL)
			{
				parser_error(itr, "|", is_valid);
				return (node);
			}
			node = node_new(ND_PIPE, node, rhs);
		}
		else
			return (node);
	}
	return (node);
}

/*  create abstract syntax tree  */
static t_node	*create_astree(t_list **itr, bool *is_valid)
{
	t_node	*node;

	if (*itr == NULL)
		return (NULL);
	node = create_sub_astree(itr, is_valid);
	while (*is_valid)
	{
		if (consume_node_kind(itr, ";", is_valid))
			node = node_new(ND_SEMICOLON, node, \
				create_sub_astree(itr, is_valid));
		else if (consume_node_kind(itr, "&&", is_valid))
			node = node_new(ND_DAND, node, create_sub_astree(itr, is_valid));
		else if (consume_node_kind(itr, "||", is_valid))
			node = node_new(ND_DPIPE, node, create_sub_astree(itr, is_valid));
		else
			return (node);
	}
	return (node);
}

int	parser(t_node **tree, t_list *token_list)
{
	bool	is_valid;

	is_valid = true;
	*tree = create_astree(&token_list, &is_valid);
	return (!is_valid);
}
