#include "minishell.h"

// <<< などを弾く
static bool	is_valid_str(char *p)
{
	bool	is_valid;
	int		idx;

	is_valid = true;
	idx = 0;
	if (p[0] && ft_strchr(PAREN_CHAR, p[0]))
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
		lexer_error(p, idx);
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
	while (p[idx] && !ft_isspace(p[idx]) && \
		!ft_strchr(RESERVED_CHAR, p[idx]) && \
		!ft_strchr(PAREN_CHAR, p[idx]))
	{
		if (p[idx] && ft_strchr(QUOTE_CHAR, p[idx]))
		{
			quote = p[idx];
			idx++;
			while (p[idx] && p[idx] != quote)
				idx++;
			if (p[idx] != quote)
			{
				lexer_error(p, idx);
				return (-1);
			}
		}
		idx++;
	}
	return (idx);
}

bool	set_token_len_and_kind(char *p, int *len, t_token_kind *kind)
{
	if (is_valid_str(p) == false)
		return (false);
	if (ft_strchr(PROCESS_DELIM_CHAR, *p))
	{
		*len = token_reserved_len(p, PROCESS_DELIM_CHAR);
		*kind = TK_DELIM;
	}
	else if (ft_strchr(REDIRECT_CHAR, *p))
	{
		*len = token_reserved_len(p, REDIRECT_CHAR);
		*kind = TK_REDIRECT;
	}
	else if (ft_strchr(PAREN_CHAR, *p))
	{
		*len = 1;
		*kind = TK_L_PAREN + (*p == ')');
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
