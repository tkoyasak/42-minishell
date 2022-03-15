#include "minishell.h"

size_t	get_word_len(char *str, bool in_squote, bool in_dquote)
{
	char	*head;

	head = str;
	if ((in_squote && *str == '\'') || (!in_squote && *str == '\"'))
		return (1);
	if (in_squote)
		return (ft_strchr(str, '\'') - str);
	if (*str == '$')
	{
		str++;
		if (*str == '?')
			return (2);
		while (*str && ft_isalnum(*str))
			str++;
	}
	else if (in_dquote)
		while (*str && *str != '$' && *str != '\"')
			str++;
	else if(ft_isspace(*str))
		while (*str && ft_isspace(*str))
			str++;
	else
		while (*str && !ft_strchr("\'\"$ ", *str))
			str++;
	return (str - head);
}

t_list	*extract_word(char **str, bool in_squote, bool in_dquote, t_expansion_kind kind)
{
	t_list		*new;
	t_expansion	*exp;

	exp = ft_calloc(1, sizeof(t_expansion));
	exp->str = ft_substr(*str, 0, get_word_len(*str, in_squote, in_dquote));
	exp->len = ft_strlen(exp->str);
	*str += exp->len;
	exp->in_squote = in_squote;
	exp->in_dquote = in_dquote;
	exp->kind = kind;
	if (exp->str && exp->str[0] == '$' && (ft_isalnum(exp->str[1]) || exp->str[1] == '?') && !in_squote)
		exp->kind = ENV;
	if (exp->str && exp->str[0] == ' ' && !in_squote && !in_dquote)
		exp->kind = NAKED_SPACE;
	new = ft_lstnew(exp);
	return (new);
}

t_list	*split_str(char *str, bool par_in_dquote)
{
	t_list	*head;
	bool	in_dquote;

	if (!str)
		return (NULL);
	head = NULL;
	if (*str == '\0')
		ft_lstadd_back(&head, extract_word(&str, false, false, NAKED_SPACE));
	in_dquote = false;
	while (*str)
	{
		if (!in_dquote && *str == '\'')
		{
			ft_lstadd_back(&head, extract_word(&str, true, par_in_dquote | in_dquote, SQUOTE));
			ft_lstadd_back(&head, extract_word(&str, true, par_in_dquote | in_dquote, STRING));
			ft_lstadd_back(&head, extract_word(&str, true, par_in_dquote | in_dquote, SQUOTE));
		}
		else if (*str == '\"')
		{
			in_dquote ^= 1;
			ft_lstadd_back(&head, extract_word(&str, false, true, DQUOTE));
		}
		else
			ft_lstadd_back(&head, extract_word(&str, false, par_in_dquote | in_dquote, STRING));
	}
	return (head);
}

t_list	*get_expansion_list(char *str, bool par_in_dquote, t_shell_var *shell_var)
{
	t_list	*head;
	t_list	*itr;
	t_list	*prev;
	t_list	*next;
	t_expansion	*exp;

	prev = NULL;
	head = split_str(str, par_in_dquote);
	itr = head;
	while (itr)
	{
		next = itr->next;
		exp = (t_expansion *)(itr->content);
		if (exp->kind == ENV)
		{
			exp->str = get_env_value(exp->str + 1, shell_var);
			// printf("97: %s\n", exp->str);
			if (prev == NULL)
				head = get_expansion_list(exp->str, exp->in_dquote, shell_var);
			else
				prev->next = get_expansion_list(exp->str, exp->in_dquote, shell_var);
			prev = ft_lstlast(head);
			prev->next = next;
		}
		else
			prev = itr;
		itr = next;
	}
	return (head);
}

t_list	*remove_quotes(t_list *src_list)
{
	t_list	*head;
	t_list	*itr;
	t_list	*next;
	t_expansion	*exp;

	head = NULL;
	itr = src_list;
	while (itr)
	{
		next = itr->next;
		itr->next = NULL;
		exp = (t_expansion *)(itr->content);
		if (exp->kind == SQUOTE || exp->kind == DQUOTE)
			; // exp_strlist_delone(itr);
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
	while (itr && ((t_expansion *)(itr->content))->kind != NAKED_SPACE)
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
	while (itr && ((t_expansion *)(itr->content))->kind != NAKED_SPACE)
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
t_list *convert_to_token(t_list *expansion_list)
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

	// token->strをt_exp_strlist_typeで分割・分類する
	// 環境変数の展開
	expansion_list = get_expansion_list(((t_token *)(token_list->content))->str, false, shell_var); // t_expansionのリスト
	expansion_list = remove_quotes(expansion_list); // t_expansionのリスト
	expanded_token_list = convert_to_token(expansion_list); // t_tokenのリスト
	return (expanded_token_list);
}

void	handle_process(t_node *node, t_shell_var *shell_var)
{
	t_list	*itr; //token_list_itr
	t_list	*next;
	t_list	*prev;

	itr = node->token_list;
	prev = NULL;
	while (itr)
	{
		next = itr->next;
		if (((t_token *)(itr->content))->kind == TK_STRING)
			itr = get_expanded_token(itr, shell_var);
		if (itr == NULL)
		{
			itr = next;
			if (prev)
				prev->next = itr;
			else
				node->token_list = itr;
			continue ;
		}
		if (prev)
			prev->next = itr;
		else
			node->token_list = itr;
		while (itr->next != NULL && itr->next != next)
			itr = itr->next;
		itr->next = next;
		prev = itr;
		itr = itr->next;
	}
}

void	expansion_sub(t_node *node, t_shell_var *shell_var)
{
	if (node->kind == ND_PROCESS)
		handle_process(node, shell_var);
	if (node->lhs)
		expansion_sub(node->lhs, shell_var);
	if (node->rhs)
		expansion_sub(node->rhs, shell_var);
}

t_node	*expansion(char *argv, t_shell_var *shell_var)
{
	t_node	*tree;

	// shell_var = ft_calloc(1, sizeof(t_shell_var));
	// shell_var->env_list = init_envlist();

	// t_list *itr = shell_var->env_list;
	// while(itr)
	// {
	// 	printf("%s\n", ((t_env *)(itr->content))->key);
	// 	itr = itr->next;
	// }
	tree = parser(argv);
	expansion_sub(tree, shell_var);
	return (tree);
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
