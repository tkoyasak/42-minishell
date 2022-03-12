#include "lexer.h"

// <<< などを弾く
static bool	is_valid_str(char *p)
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
static int	token_reserved_len(char *p, char *defined_char)
{
	int	idx;

	idx = 0;
	while (p[idx] && ft_strchr(defined_char, p[idx]))
		idx++;
	return (idx);
}

/*  return length of string except for RESERVED_CHAR "<>|&;"  */
static int	token_string_len(char *p)
{
	int		idx;
	char	quote;

	idx = 0;
	while (p[idx] && !ft_isspace(p[idx]) && !ft_strchr(RESERVED_CHAR, p[idx]))
	{
		if (p[idx] && ft_strchr(QUOTE_CHAR, p[idx]))
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

static bool	set_token_len_and_kind(char *p, int *len, t_token_kind *kind)
{
	if (is_valid_str(p) == false)
		return (false);
	if (ft_strchr(PROCESS_DELIM_CHAR, *p))
	{
		*len = token_reserved_len(p, PROCESS_DELIM_CHAR);
		*kind = TK_PROCESS_DELIM;
	}
	else if (ft_strchr(REDIRECT_CHAR, *p))
	{
		*len = token_reserved_len(p, REDIRECT_CHAR);
		*kind = TK_REDIRECT;
	}
	else
	{
		*len = token_string_len(p);
		*kind = TK_STRING;
	}
	if (*len == -1)
		return (false);
	return (true);
}

/*  create new token and advance *p by len  */
static t_list	*consume_new_token(t_list *itr,
									t_token_kind kind, char **p, int len)
{
	t_token	*token;

	token = ft_calloc(1, sizeof(t_token));
	token->kind = kind;
	token->str = ft_strndup(*p, len);
	*p += len;
	itr->next = ft_lstnew(token);
	return (itr->next);
}

/*  スペースで分割し、token(TK_PROCESS_DELIM, TK_REDIRECT, TK_STRING)に分割  */
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
			itr = consume_new_token(kind, &p, len, itr);
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
