#include "expansion.h"

t_exp_strlist	*exp_strlist_last(t_exp_strlist *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

void	exp_strlist_add_back(t_list **lst, t_list *newsrc)
{
	if (!lst)
		return ;
	if (!*lst)
		*lst = newsrc;
	else
		(exp_strlist_last(*lst))->next = newsrc;
}

t_exp_strlist	*exp_strlist_new(char *str, bool in_squote, bool in_dquote, t_exp_strlist_type type)
{
	t_exp_strlist	*new;

	new = malloc(sizeof(t_exp_strlist));
	if (!new)
		return (NULL);
	new->str = str;
	new->in_squote = in_squote;
	new->in_dquote = in_dquote;
	new->type = type;
	if (str && str[0] == '$' && ft_isalnum(str[1]) && !in_squote)
		new->type = ENV;
	if (str && str[0] == ' ' && !in_squote && !in_dquote)
		new->type = NAKED_SPACE;
	new->next = NULL;
	return (new);
}

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

t_exp_strlist	*extract_word(char **str, bool in_squote, bool in_dquote, t_exp_strlist_type type)
{
	t_exp_strlist	*new;
	char			*new_word;

	new_word = ft_substr(*str, 0, get_word_len(*str, in_squote, in_dquote));
	new = exp_strlist_new(new_word, in_squote, in_dquote, type);
	*str += ft_strlen(new_word);
	return (new);
}

t_exp_strlist	*get_exp_strlist(char *str)
{
	t_exp_strlist	*head;
	bool			in_dquote;

	head = NULL;
	in_dquote = false;
	while (*str)
	{
		if (!in_dquote && *str == '\'')
		{
			exp_strlist_add_back(&head, extract_word(&str, true, in_dquote, SQUOTE));
			exp_strlist_add_back(&head, extract_word(&str, true, in_dquote, STRING));
			exp_strlist_add_back(&head, extract_word(&str, true, in_dquote, SQUOTE));
		}
		else if (*str == '\"')
		{
			in_dquote ^= 1;
			exp_strlist_add_back(&head, extract_word(&str, false, true, DQUOTE));
		}
		else
			exp_strlist_add_back(&head, extract_word(&str, false, in_dquote, STRING));
	}
	return (head);
}

// expansion前のトークン１つを受け取って、展開して新しいトークン列を返す
t_token	*get_expanded_token(t_token *token)
{
	t_token			*expanded_token;
	t_exp_strlist	*exp_strlist;

	// token->strをt_exp_strlist_typeで分割・分類する
	exp_strlist = get_exp_strlist(token->str);


}

void	handle_process(t_node *node)
{
	t_token	*token;
	t_token	*next;
	t_token	*prev;

	token = node->token;
	prev = NULL;
	while (token)
	{
		next = token->next;
		token = get_expanded_token(token);
		if (prev)
			prev->next = token;
		else
			node->token = token;
		while (token->next != NULL && token->next != next && token->next->kind != TK_EOF)
		{
			token = token->next;
		}
		token->next = next;
		prev = token;
		token = token->next;
	}
}

void	expansion_sub(t_node *node)
{
	//今のnodeに対する処理
	if (node->kind == ND_PROCESS)
		handle_process(node);
	if (node->lhs)
		expansion_sub(node->lhs);
	if (node->rhs)
		expansion_sub(node->rhs);
}

t_node	*expansion(char *argv)
{
	t_token	*tokens;
	t_node	*tree;

	tree = parser(argv);
	expansion_sub(tree);
	return (tree);
}

static void	dfs(t_node *tree)
{
	if (tree->lhs)
		dfs(tree->lhs);
	printf("kind:%d\n", tree->kind);
	while (tree->token)
	{
		printf("%s\n", tree->token->str);
		tree->token = tree->token->next;
	}
	if (tree->rhs)
		dfs(tree->rhs);
}

// int	main(void)
// {
// 	// char *s = "ls -al";

// 	// // int	len = get_expanded_len(s);
// 	// // printf("%s : %d\n", s, len);
// 	// t_node	*tree;

// 	// tree = expansion("ls -al | cat");
// 	// dfs(tree);
// 	// return (0);
// 	printf("%s\n", getenv("ppp"));
// 	return (0);
// }
