#include "minishell.h"

size_t	get_word_len_heredoc(char *str, bool in_squote, bool in_dquote)
{
	char	*head;

	(void)in_squote;
	(void)in_dquote;
	head = str;
	if (*str == '$')
	{
		str++;
		while (*str && ft_isalnum(*str))
			str++;
	}
	else
		while (*str && *str != '$')
			str++;
	return (str - head);
}

t_list	*split_str_heredoc(char *str, bool par_in_dquote)
{
	t_list	*head;
	bool	in_dquote;

	if (!str)
		return (NULL);
	head = NULL;
	if (*str == '\0')
		ft_lstadd_back(&head, extract_word_heredoc(&str, false, \
				par_in_dquote, STRING));
	in_dquote = false;
	while (*str)
	{
		ft_lstadd_back(&head, extract_word_heredoc(&str, false, \
				par_in_dquote | in_dquote, STRING));
	}
	return (head);
}

size_t	get_expanded_len_heredoc(t_list *exp_list)
{
	size_t	len;

	len = 0;
	while (exp_list)
	{
		len += ((t_expd *)(exp_list->content))->len;
		exp_list = exp_list->next;
	}
	return (len);
}


char	*remove_quote_heredoc(char *limiter, bool *in_quote)
{
	char	*dst;
	size_t	src_idx;
	size_t	dst_idx;

	dst = ft_xcalloc(ft_strlen(limiter) + 1, sizeof(char));
	src_idx = 0;
	dst_idx = 0;
	while (limiter[src_idx])
	{
		if (ft_strchr(QUOTE_CHAR, limiter[src_idx]))
		{
			*in_quote = true;
			src_idx++;
			continue ;
		}
		dst[dst_idx++] = limiter[src_idx++];
	}
	return (dst);
}
