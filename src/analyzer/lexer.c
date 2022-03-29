/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoyasak <tkoyasak@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/25 10:48:26 by tkoyasak          #+#    #+#             */
/*   Updated: 2022/03/29 15:12:25 by tkoyasak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	lexer_error(char *p, int size)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", \
		STDERR_FILENO);
	ft_putnchar_fd(p, size, STDERR_FILENO);
	ft_putstr_fd("'\n", STDERR_FILENO);
}

/*  create new token and advance *p by len  */
static t_list	*consume_new_token(t_list *itr,
									char **p, int len, t_token_kind kind)
{
	t_token	*token;

	token = ft_xcalloc(1, sizeof(t_token));
	token->kind = kind;
	token->str = ft_xstrndup(*p, len);
	*p += len;
	itr->next = ft_xlstnew(token);
	return (itr->next);
}

/*  split by space and tokenize.  */
static t_list	*tokenize(char *p)
{
	t_list			head;
	t_list			*itr;
	int				len;
	t_token_kind	kind;

	head.next = NULL;
	itr = &head;
	while (*p)
	{
		if (ft_isspace(*p))
		{
			p++;
			continue ;
		}
		if (set_token_len_and_kind(p, &len, &kind))
		{
			itr = consume_new_token(itr, &p, len, kind);
			continue ;
		}
		ft_lstclear(&head.next, delete_token);
		return (NULL);
	}
	return (head.next);
}

static int	validate_syntax(t_list *token_list)
{
	t_list			*itr;
	t_token_kind	kind;
	int				parenthesis;
	char			p;

	itr = token_list;
	parenthesis = 0;
	while (itr)
	{
		kind = ((t_token *)itr->content)->kind;
		if (kind == TK_L_PAREN || kind == TK_R_PAREN)
		{
			parenthesis += 1 - 2 * (kind - TK_L_PAREN);
			p = '(' + (kind - TK_L_PAREN);
		}
		if (parenthesis < 0)
			break ;
		itr = itr->next;
	}
	if (parenthesis != 0)
	{
		lexer_error(&p, 1);
		return (1);
	}
	return (0);
}

int	lexer(char *line, t_list **token_list)
{
	*token_list = tokenize(line);
	if (*token_list == NULL)
		return (1);
	if (validate_syntax(*token_list))
	{
		ft_lstclear(token_list, delete_token);
		return (1);
	}
	return (0);
}
