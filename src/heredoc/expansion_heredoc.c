#include "minishell.h"

t_list	*extract_word_heredoc(char **str, bool in_squote, bool in_dquote, t_expansion_kind kind)
{
	t_list		*new;
	t_expansion	*exp;

	exp = ft_xcalloc(1, sizeof(t_expansion));
	exp->str = \
		ft_xsubstr(*str, 0, get_word_len_heredoc(*str, in_squote, in_dquote));
	exp->len = ft_strlen(exp->str);
	*str += exp->len;
	exp->in_squote = in_squote;
	exp->in_dquote = in_dquote;
	exp->kind = kind;
	if (exp->str && exp->str[0] == '$' && ft_isalnum(exp->str[1]) && !in_squote)
		exp->kind = ENV;
	new = ft_xlstnew(exp);
	return (new);
}

// prevがNULLの時とそれ以外の場合で、listの繋ぎかえ
void	connect_expansion_list_heredoc(t_expansion *exp, t_list *head, t_list *prev, t_shell_var *shell_var)
{
	if (prev == NULL)
		head = get_expansion_list_heredoc(exp->str, exp->in_dquote, shell_var);
	else
		prev->next = \
				get_expansion_list_heredoc(exp->str, exp->in_dquote, shell_var);
	prev = ft_lstlast(head);
}

t_list	*get_expansion_list_heredoc(char *str, bool par_in_dquote, t_shell_var *shell_var)
{
	t_list		*head;
	t_list		*itr;
	t_list		*prev;
	t_list		*next;
	t_expansion	*exp;

	prev = NULL;
	head = split_str_heredoc(str, par_in_dquote);
	itr = head;
	while (itr)
	{
		next = itr->next;
		exp = (t_expansion *)(itr->content);
		if (exp->kind == ENV)
		{
			exp->str = get_env_value_str(exp->str + 1, shell_var);
			connect_expansion_list_heredoc(exp, head, prev, shell_var);
			prev->next = next;
		}
		else
			prev = itr;
		itr = next;
	}
	return (head);
}

/*  expand env word in here document like aa$PATH  */
char	*expansion_heredoc(char *str, t_shell_var *shell_var)
{
	char	*dst;
	char	*cur;
	size_t	len;
	t_list	*exp_list;

	exp_list = get_expansion_list_heredoc(str, false, shell_var);
	len = get_expanded_len_heredoc(exp_list);
	dst = (char *)ft_xmalloc(sizeof(char) * (len + 1));
	dst[0] = '\0';
	while (exp_list)
	{
		cur = ((t_expansion *)(exp_list->content))->str;
		ft_strlcat(dst, cur, ft_strlen(dst) + ft_strlen(cur) + 1);
		exp_list = exp_list->next;
	}
	return (dst);
}
