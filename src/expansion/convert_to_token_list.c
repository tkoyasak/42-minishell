#include "minishell.h"

t_list	*remove_quotes(t_list *src_list)
{
	t_list		*head;
	t_list		*itr;
	t_list		*next;
	t_expd	*exp;

	head = NULL;
	itr = src_list;
	while (itr)
	{
		next = itr->next;
		itr->next = NULL;
		exp = (t_expd *)(itr->content);
		if (exp->kind == SQUOTE || exp->kind == DQUOTE)
			ft_lstdelone(itr, delete_expansion);
		else
			ft_lstadd_back(&head, itr);
		itr = next;
	}
	return (head);
}

static size_t	token_str_len(t_list *src_list)
{
	size_t		len;
	t_list		*itr;
	t_expd	*exp;

	len = 0;
	itr = src_list;
	if (((t_expd *)(itr->content))->kind == FILENAME_EXPANSION)
		return (((t_expd *)(itr->content))->len);
	while (itr && ((t_expd *)(itr->content))->kind != NAKED_SPACE && \
		((t_expd *)(itr->content))->kind != FILENAME_EXPANSION)
	{
		exp = (t_expd *)(itr->content);
		len += exp->len;
		itr = itr->next;
	}
	return (len);
}

static char	*consume_token_str_join(t_list **src_list, char *buf)
{
	t_list		*itr;
	size_t		len;
	t_expd	*exp;

	len = 0;
	itr = *src_list;
	if (((t_expd *)(itr->content))->kind == FILENAME_EXPANSION)
	{
		exp = (t_expd *)(itr->content);
		ft_strlcat(buf, exp->str, exp->len + 1);
		*src_list = (*src_list)->next;
		return (buf);
	}
	while (itr && ((t_expd *)(itr->content))->kind != NAKED_SPACE && \
		((t_expd *)(itr->content))->kind != FILENAME_EXPANSION)
	{
		exp = (t_expd *)(itr->content);
		len += exp->len;
		ft_strlcat(buf, exp->str, len + 1);
		itr = itr->next;
	}
	*src_list = itr;
	return (buf);
}

static t_list	*consume_new_joined_token(t_list **itr)
{
	size_t		len;
	char		*str;
	t_token		*token;

	token = ft_xcalloc(1, sizeof(t_token));
	len = token_str_len(*itr);
	str = ft_xcalloc(len + 1, sizeof(char));
	token->str = consume_token_str_join(itr, str);
	token->kind = TK_STRING;
	return (ft_xlstnew(token));
}

// naked spaceで分割し、文字列(t_exp_strlist)を連結してトークン化する
t_list	*convert_to_token_list(t_list *expansion_list)
{
	t_list		*head;
	t_list		*itr;
	t_expd	*exp;

	head = NULL;
	itr = expansion_list;
	while (itr)
	{
		exp = (t_expd *)(itr->content);
		if (exp->kind == NAKED_SPACE)
			itr = itr->next;
		else
			ft_lstadd_back(&head, consume_new_joined_token(&itr));
	}
	return (head);
}
