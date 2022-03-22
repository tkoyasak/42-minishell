#include "minishell.h"

size_t	token_str_len(t_list *src_list)
{
	size_t		len;
	t_list		*itr;
	t_expansion	*exp;

	len = 0;
	itr = src_list;
	if (((t_expansion *)(itr->content))->kind == FILENAME_EXPANSION)
		return (((t_expansion *)(itr->content))->len);
	while (itr && ((t_expansion *)(itr->content))->kind != NAKED_SPACE && \
		((t_expansion *)(itr->content))->kind != FILENAME_EXPANSION)
	{
		exp = (t_expansion *)(itr->content);
		len += exp->len;
		itr = itr->next;
	}
	return (len);
}

char	*token_str_join(t_list **src_list, char *buf)
{
	t_list		*itr;
	size_t		len;
	t_expansion	*exp;

	len = 0;
	itr = *src_list;
	if (((t_expansion *)(itr->content))->kind == FILENAME_EXPANSION)
	{
		exp = (t_expansion *)(itr->content);
		ft_strlcat(buf, exp->str, exp->len + 1);
		*src_list = (*src_list)->next;
		return (buf);
	}
	while (itr && ((t_expansion *)(itr->content))->kind != NAKED_SPACE && \
		((t_expansion *)(itr->content))->kind != FILENAME_EXPANSION)
	{
		exp = (t_expansion *)(itr->content);
		len += exp->len;
		ft_strlcat(buf, exp->str, len + 1);
		itr = itr->next;
	}
	*src_list = itr;
	return (buf);
}

// naked spaceで分割し、文字列(t_exp_strlist)を連結してトークン化する
t_list	*convert_to_token_list(t_list *expansion_list)
{
	t_list		*head;
	t_list		*itr;
	size_t		len;
	char		*str;
	t_expansion	*exp;
	t_token		*token;

	// expansion_list = remove_quotes(expansion_list);
	head = NULL;
	itr = expansion_list;
	while (itr)
	{
		exp = (t_expansion *)(itr->content);
		if (exp->kind == NAKED_SPACE)
		{
			itr = itr->next;
			continue ;
		}
		token = ft_calloc(1, sizeof(t_token));
		len = token_str_len(itr);
		str = ft_calloc(len + 1, sizeof(char));
		token->str = token_str_join(&itr, str);
		token->kind = TK_STRING;
		ft_lstadd_back(&head, ft_lstnew(token));
	}
	return (head);
}
