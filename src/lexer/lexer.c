#include "lexer.h"

// <<< などを弾く
bool	is_valid_str(char *p)
{
	int	idx;

	idx = 0;
	while (p[idx] && ft_strchr(RESERVED_CHAR, p[idx]))
		idx++;
	if (idx == 0)
		return (true);
	if (idx > 2)
		return (false);
	if (idx == 2 && p[0] != p[1]) // 変更 &> >& &< <& はOK 1>file  1&>file
		return (false);
	if (idx == 2 && p[0] == ';')
		return (false);
	if (idx == 1 && p[0] == '&')
		return (false);
	return (true);
}

/*  return length of RESERVED_CHAR "<>|&;"  */
int	reserved_len(char *p)
{
	int	idx;

	idx = 0;
	while (p[idx] && ft_strchr(RESERVED_CHAR, p[idx]))
		idx++;
	return (idx);
}

/*  return length of string except for RESERVED_CHAR "<>|&;"  */
int	string_len(char *p)
{
	int		idx;
	char	quote;

	idx = 0;
	while (p[idx] && ft_isspace(p[idx]) == false && ft_strchr(RESERVED_CHAR, p[idx]) == false)
	{
		if (p[idx] && ft_strchr("\"'", p[idx]))
		{
			quote = p[idx];
			idx++;
			while (p[idx] && p[idx] != quote)
				idx++;
			if (p[idx] != quote)
				return (-1);
		}
		idx++;
	}
	return (idx);
}

/*  create new token and advance *p by len  */
t_list	*new_token_consume(t_token_kind kind, t_list *cur, char **p, int len)
{
	t_token	*token;

	token = ft_calloc(1, sizeof(t_token));
	token->kind = kind;
	token->str = ft_calloc(len + 1, sizeof(char));
	strlcpy(token->str, *p, len + 1);
	*p += len;
	cur->next = ft_lstnew(token);
	return (cur->next);
}

/*  スペースで分割し、token(TK_RESERVED, TK_REDIRECT, TK_STRING)に分割  */
t_list	*tokenize(char *p)
{
	t_list	head;
	t_list	*cur;
	int		len;

	head.next = NULL;
	cur = &head;
	while (*p)
	{
		if (ft_isspace(*p))
		{
			p++;
			continue;
		}
		if (is_valid_str(p) == false)
		{
			exit(1);
		}
		len = reserved_len(p);
		if (len > 0)
		{
			if (ft_strchr("<>", *p))
				cur = new_token_consume(TK_REDIRECT, cur, &p, len);
			else
				cur = new_token_consume(TK_RESERVED, cur, &p, len);
			continue ;
		}
		len = string_len(p);
		if (len > 0)
		{
			cur = new_token_consume(TK_STRING, cur, &p, len);
			continue ;
		}
		// free必要 error_handler(free_all(&head, ___));
		printf("quotes error\n");
		exit(1);
	}
	return (head.next);
}

t_list	*lexer(char *line)
{
	t_list	*token_list;

	token_list = tokenize(line);
	return (token_list);
}
