#include "expansion.h"

int	env_word_len(char *str)
{
	int	idx;

	idx = 0;
	while (str[idx] && isalnum(str[idx]))
		idx++;
	return (idx);
}

t_exp_strlist	*tk_strlist_last(t_exp_strlist *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

void	tk_strlist_add_back(t_list **lst, t_list *newsrc)
{
	if (!lst)
		return ;
	if (!*lst)
		*lst = newsrc;
	else
		(tk_strlist_last(*lst))->next = newsrc;
}

t_exp_strlist	*tk_strlist_new(char *str)
{
	t_exp_strlist	*new;

	new = malloc(sizeof(t_exp_strlist));
	new->str = str;
	new->next = NULL;
	return (new);
}

t_exp_strlist	*expand_tk_string(char *str)
{
	t_exp_strlist	*head;
	bool			has_squote;
	bool			has_dquote;
	char			*env_word;
	size_t			envword_len;
	int				idx;
	int				left_idx;

	if (str == NULL)
		return (NULL);
	head = NULL;
	idx = 0;
	left_idx = 0;
	while (str[idx])
	{
		if (!has_dquote && str[idx] == '\'')
			has_squote ^= true;
		else if (!has_squote && str[idx] == '"')
			has_dquote ^= true;
		if (!has_squote && str[idx] == '$' && str[idx + 1])
		{
			tk_strlist_add_back(&head, tk_strlist_new(ft_substr(str, left_idx, idx - left_idx)));
			idx++;
			envword_len = env_word_len(&str[idx]);
			env_word = ft_substr(str, idx, envword_len);
			tk_strlist_add_back(&head, expand_tk_string(getenv(env_word)));
			idx += envword_len;
			left_idx = idx;
		}
		else
			idx++;
	}
	if (left_idx < idx)
		tk_strlist_add_back(&head, tk_strlist_new(ft_substr(str, left_idx, idx - left_idx)));
	return (head);
}

size_t	get_raw_len(char *str)
{
	char	quote;
	size_t	len;
	size_t	idx;

	len = 0;
	idx = 0;
	while (str[idx])
	{
		if (str[idx] && strchr("\"'", str[idx]))
		{
			quote = str[idx];
			idx++;
			while (str[idx] && str[idx] != quote)
			{
				len++;
				idx++;
			}
			if (str[idx] != quote)
				return (0);
			idx++;
		}
		else
		{
			idx++;
			len++;
		}
	}
	return (len);
}

char	*copy_raw_str(char *src, size_t len)
{
	char	*dst;
	char	quote;
	size_t	src_idx;
	size_t	dst_idx;

	dst = malloc(sizeof(char) * (len + 1));
	src_idx = 0;
	dst_idx = 0;
	while (src[src_idx])
	{
		if (src[src_idx] && strchr("\"'", src[src_idx]))
		{
			quote = src[src_idx];
			src_idx++;
			while (src[src_idx] && src[src_idx] != quote)
				dst[dst_idx++] = src[src_idx++];
			if (src[src_idx] != quote)
				return (NULL);
			src_idx++;
		}
		else
			dst[dst_idx++] = src[src_idx++];
	}
	dst[dst_idx] = '\0';
	return (dst);
}

void	remove_quotes(t_token *token)
{
	t_token	*itr;
	size_t	len;
	char	raw_str;

	itr = token;
	while (itr->kind != TK_EOF)
	{
		printf("149: %s\n", itr->str);
		len = get_raw_len(token->str);
		token->str = copy_raw_str(token->str, len);
		itr = itr->next;
	}
}

// expansion前のトークン１つを受け取って、展開して新しいトークン列を返す
t_token	*get_expanded_token(t_token *token)
{
	char		*expanded_str;
	size_t		expanded_len;
	t_exp_strlist	*head_strlist;
	t_exp_strlist	*itr;
	t_token		*expanded_token;

	expanded_len = 0;
	head_strlist = expand_tk_string(token->str);
	itr = head_strlist;
	while (itr)
	{
		itr->len = strlen(itr->str);
		expanded_len += itr->len;
		itr = itr->next;
	}
	expanded_str = malloc(expanded_len + 1);
	itr = head_strlist;
	expanded_len = 0;
	while (itr)
	{
		expanded_len += itr->len;
		strlcat(expanded_str, itr->str, expanded_len + 1);
		itr = itr->next;
	}
	expanded_token = lexer(expanded_str, true);
	remove_quotes(expanded_token);
	return (expanded_token);
}

void	handle_process(t_node *node)
{
	t_token	*token;
	t_token *next;
	t_token *prev;

	token = node->token;
	prev = NULL;
	while (token)
	{
		next = token->next;
		token = get_expanded_token(token); //(token)を展開して先頭の(token)アドレスを返す
		if (prev)
			prev->next = token;
		else
			node->token = token;
		while (token->next != NULL && token->next != next && token->next->kind != TK_EOF)
			token = token->next;
		assert(token->kind != TK_EOF);
		token->next = next;
		prev = token;
		token = token->next;
	}
}

void	expansion_sub(t_node *node)
{
	//今のnodeに対する処理
	if (node->kind == ND_PROCESS)
	{
		handle_process(node); // cmd > cmd = process
	}
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
