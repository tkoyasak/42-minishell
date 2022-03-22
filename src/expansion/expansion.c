#include "minishell.h"

t_list	*remove_quotes(t_list *src_list)
{
	t_list		*head;
	t_list		*itr;
	t_list		*next;
	t_expansion	*exp;

	head = NULL;
	itr = src_list;
	while (itr)
	{
		next = itr->next;
		itr->next = NULL;
		exp = (t_expansion *)(itr->content);
		if (exp->kind == SQUOTE || exp->kind == DQUOTE)
			ft_lstdelone(itr, delete_expansion);
		else
			ft_lstadd_back(&head, itr);
		itr = next;
	}
	return (head);
}

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
t_list	*convert_to_token(t_list *expansion_list)
{
	t_list	*head; // token_list
	t_list	*itr;
	size_t	len;
	char	*str;
	t_expansion	*exp;
	t_token		*token;

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

// expansion前のトークン１つを受け取って、展開して新しいトークン列を返す
t_list	*get_expanded_token(t_list *token_list, t_shell_var *shell_var)
{
	t_list			*expansion_list;
	t_list			*expanded_token_list;

	// token_list->next = NULL;
	// token->strをt_exp_strlist_typeで分割・分類する
	// 環境変数の展開
	expansion_list = get_expansion_list(((t_token *)(token_list->content))->str, false, shell_var); // t_expansionのリスト
	// *のfilename expansion展開
	expansion_list = get_filename_expansion(expansion_list);
	expansion_list = remove_quotes(expansion_list); // t_expansionのリスト
	expanded_token_list = convert_to_token(expansion_list); // t_tokenのリスト
	ft_lstdelone(token_list, delete_token);
	ft_lstclear(&expansion_list, delete_expansion);
	return (expanded_token_list);
}

void	handle_process(t_list **token_list, t_shell_var *shell_var)
{
	t_list	head;
	t_list	*itr;
	t_list	*next;
	t_list	*prev;
	bool	is_delimiter;

	head.next = *token_list;
	itr = *token_list;
	prev = &head;
	is_delimiter = false;
	while (itr)
	{
		next = itr->next;
		itr->next = NULL;
		if (!is_delimiter && ((t_token *)(itr->content))->kind == TK_STRING)
		{
			itr = get_expanded_token(itr, shell_var);
			if (itr == NULL)
			{
				itr = next;
				prev->next = itr;
				continue ;
			}
			prev->next = itr;
			itr = ft_lstlast(itr);
		}
		else
		{
			is_delimiter = false;
			if (((t_token *)(itr->content))->kind == TK_REDIRECT && \
				ft_strcmp(((t_token *)(itr->content))->str, "<<") == 0)
				is_delimiter = true;
		}
		itr->next = next;
		prev = itr;
		itr = itr->next;
	}
	*token_list = head.next;
}

void	expansion(t_expression *expression, t_shell_var *shell_var)
{
	t_list	*itr;

	itr = expression->process_list;
	while(itr)
	{
		handle_process(&((t_process *)(itr->content))->token_list, shell_var);
		g_exit_status = 0; // bashの挙動に合わせた
		itr = itr->next;
	}
}

// static void	dfs(t_node *tree)
// {
// 	if (tree->lhs)
// 		dfs(tree->lhs);
// 	printf("282:kind:%d\n", tree->kind);
// 	t_list *itr = (t_list *)(tree->token_list);
// 	while (itr)
// 	{
// 		printf("263: %s\n", ((t_token *)(itr->content))->str);
// 		itr = itr->next;
// 	}
// 	if (tree->rhs)
// 		dfs(tree->rhs);
// }

// int	main(void)
// {
// 	// char *s = "ls -al";

// 	// int	len = get_expanded_len(s);
// 	// printf("%s : %d\n", s, len);
// 	t_node	*tree;

// 	printf("\"ls -al\"aaaa fff | cat\n");
// 	tree = expansion("\"ls -al\"aaaa fff | cat");
// 	dfs(tree);
// 	return (0);
// }
