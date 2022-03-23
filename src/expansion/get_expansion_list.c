#include "minishell.h"

static size_t	get_word_len(char *str, bool in_squote, bool in_dquote)
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
		while (*str && (ft_isalnum(*str) || *str == '_'))
			str++;
	}
	else if (in_dquote)
		while (*str && *str != '$' && *str != '\"')
			str++;
	else if (ft_isspace(*str))
		while (*str && ft_isspace(*str))
			str++;
	else
		while (*str && !ft_strchr("\'\"$ ", *str))
			str++;
	return (str - head);
}

static t_list	*extract_word(char **str, bool in_squote, bool in_dquote, t_expd_kind kind)
{
	t_expd	*exp;

	exp = ft_xcalloc(1, sizeof(t_expd));
	exp->str = ft_xsubstr(*str, 0, get_word_len(*str, in_squote, in_dquote));
	exp->len = ft_strlen(exp->str);
	*str += exp->len;
	exp->in_squote = in_squote;
	exp->in_dquote = in_dquote;
	exp->kind = kind;
	if (!in_squote && exp->str && exp->str[0] == '$' && \
		(ft_isalnum(exp->str[1]) || exp->str[1] == '_' || exp->str[1] == '?'))
		exp->kind = ENV;
	if (!in_squote && !in_dquote && exp->str && exp->str[0] == ' ')
		exp->kind = NAKED_SPACE;
	return (ft_xlstnew(exp));
}

static t_list	*split_token_str(char *str, bool par_in_dquote)
{
	t_list	*head;
	bool	in_dquote;
	bool	flag;

	head = NULL;
	in_dquote = false;
	while (*str)
	{
		flag = par_in_dquote | in_dquote;
		if (!in_dquote && *str == '\'')
		{
			ft_lstadd_back(&head, extract_word(&str, true, flag, SQUOTE));
			ft_lstadd_back(&head, extract_word(&str, true, flag, STRING));
			ft_lstadd_back(&head, extract_word(&str, true, flag, SQUOTE));
		}
		else if (*str == '\"')
		{
			in_dquote ^= 1;
			ft_lstadd_back(&head, extract_word(&str, false, true, DQUOTE));
		}
		else
			ft_lstadd_back(&head, extract_word(&str, false, flag, STRING));
	}
	return (head);
}

static t_list	*get_split_token_list(char *str, bool par_in_dquote)
{
	if (!str)
		return (NULL);
	if (*str == '\0')
		return (extract_word(&str, false, par_in_dquote, STRING));
	return (split_token_str(str, par_in_dquote));
}

t_list	*get_expansion_list(char *str, bool par_in_dquote, t_shell_var *shell_var)
{
	t_list		head;
	t_list		*itr;
	t_list		*prev;
	t_list		*next;
	t_expd	*exp;

	head.next = get_split_token_list(str, par_in_dquote);
	itr = head.next;
	prev = &head;
	while (itr)
	{
		next = itr->next;
		exp = (t_expd *)(itr->content);
		if (exp->kind == ENV)
		{
			exp->str = get_env_value_str(exp->str + 1, shell_var);
			prev->next = get_expansion_list(exp->str, exp->in_dquote, shell_var);
			prev = ft_lstlast(head.next);
			prev->next = next;
		}
		else
			prev = itr;
		itr = next;
	}
	return (head.next);
}
