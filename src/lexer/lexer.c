#include "minishell.h"

// <<< などを弾く
static bool	is_valid_str(char *p)
{
	bool	is_valid;
	int		idx;

	is_valid = true;
	idx = 0;
	if (p[0] && ft_strchr(PARENTHESIS_CHAR, p[0]))
		return (true);
	while (p[idx] && ft_strchr(RESERVED_CHAR, p[idx]))
		idx++;
	if (idx == 1 && p[0] == '&')
		is_valid = false;
	if (idx == 2 && (p[0] != p[1] || p[0] == ';')) // 変更 &> >& &< <& はOK 1>file  1&>file
		is_valid = false;
	if (idx > 2)
		is_valid = false;
	if (!is_valid)
	{
		ft_putstr_fd("syntax error near unexpected token `", STDERR_FILENO);
		ft_putnchar_fd(p, idx, STDERR_FILENO);
		ft_putstr_fd("'\n", STDERR_FILENO);
	}
	return (is_valid);
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
	while (p[idx] && !ft_isspace(p[idx]) && !ft_strchr(RESERVED_CHAR, p[idx]) && !ft_strchr(PARENTHESIS_CHAR, p[idx]))
	{
		if (p[idx] && ft_strchr(QUOTE_CHAR, p[idx]))
		{
			quote = p[idx];
			idx++;
			while (p[idx] && p[idx] != quote)
				idx++;
			if (p[idx] != quote)
			{
				ft_putstr_fd("syntax error near unexpected token `", STDERR_FILENO);
				ft_putnchar_fd(p, idx, STDERR_FILENO);
				ft_putstr_fd("'\n", STDERR_FILENO);
				return (-1);
			}
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
	else if (ft_strchr(PARENTHESIS_CHAR, *p))
	{
		*len = 1;
		if (*p == '(')
			*kind = TK_L_PARENTHESIS;
		else
			*kind = TK_R_PARENTHESIS;
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
									char **p, int len, t_token_kind kind)
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
			itr = consume_new_token(itr, &p, len, kind);
			continue ;
		}
		// free必要 error_handler(free_all(&head, ___));
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
