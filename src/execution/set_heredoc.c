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

t_list	*extract_word_heredoc(char **str, bool in_squote, bool in_dquote, t_expansion_kind kind)
{
	t_list		*new;
	t_expansion	*exp;

	exp = ft_calloc(1, sizeof(t_expansion));
	exp->str = ft_substr(*str, 0, get_word_len_heredoc(*str, in_squote, in_dquote));
	exp->len = ft_strlen(exp->str);
	*str += exp->len;
	exp->in_squote = in_squote;
	exp->in_dquote = in_dquote;
	exp->kind = kind;
	if (exp->str && exp->str[0] == '$' && ft_isalnum(exp->str[1]) && !in_squote)
		exp->kind = ENV;
	new = ft_lstnew(exp);
	return (new);
}

t_list	*split_str_heredoc(char *str, bool par_in_dquote)
{
	t_list	*head;
	bool	in_dquote;

	if (!str)
		return (NULL);
	head = NULL;
	if (*str == '\0')
		ft_lstadd_back(&head, extract_word_heredoc(&str, false, par_in_dquote, STRING));
	in_dquote = false;
	while (*str)
	{
		ft_lstadd_back(&head, extract_word_heredoc(&str, false, par_in_dquote | in_dquote, STRING));
	}
	return (head);
}

t_list	*get_expansion_list_heredoc(char *str, bool par_in_dquote, t_shell_var *shell_var)
{
	t_list	*head;
	t_list	*itr;
	t_list	*prev;
	t_list	*next;
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
			if (prev == NULL)
				head = get_expansion_list_heredoc(exp->str, exp->in_dquote, shell_var);
			else
				prev->next = get_expansion_list_heredoc(exp->str, exp->in_dquote, shell_var);
			prev = ft_lstlast(head);
			prev->next = next;
		}
		else
			prev = itr;
		itr = next;
	}
	return (head);
}

size_t	get_expanded_len_heredoc(t_list *exp_list)
{
	size_t	len;

	len = 0;
	while (exp_list)
	{
		len += ((t_expansion *)(exp_list->content))->len;
		exp_list = exp_list->next;
	}
	return (len);
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
	dst = (char *)malloc(sizeof(char) * (len + 1));
	dst[0] = '\0';
	while (exp_list)
	{
		cur = ((t_expansion *)(exp_list->content))->str;
		ft_strlcat(dst, cur, ft_strlen(dst) + ft_strlen(cur) + 1);
		exp_list = exp_list->next;
	}
	return (dst);
}

char	*remove_quote_heredoc(char *limiter, bool *in_quote)
{
	char	*dst;
	size_t	src_idx;
	size_t	dst_idx;

	dst = ft_calloc(ft_strlen(limiter) + 1, sizeof(char));
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

void	set_heredoc_sub(t_process *process, char *limiter, t_shell_var *shell_var)
{
	size_t	len;
	char	*temp;
	char	*new_document;
	int		res;
	bool	in_quote;

	close(process->here_pipefd[0]);
	limiter = remove_quote_heredoc(limiter, &in_quote);
	new_document = ft_strdup("");
	if (!new_document)
		return ;
	len = ft_strlen(limiter);
	limiter[len] = '\n';
	res = get_next_line(STDIN_FILENO, &temp);
	while (temp && ft_strncmp(temp, limiter, len + 1))
	{
		if (in_quote == false)
			temp = expansion_heredoc(temp, shell_var);
		new_document = ft_strjoin(new_document, temp);
		// if (!new_document)
			// free_one(&temp);
		// free_one(&temp);
		res = get_next_line(STDIN_FILENO, &temp);
	}
	// free(limiter);
	// free_one(&temp);
	if (write(process->here_pipefd[1], new_document, ft_strlen(new_document)) == -1)
		printf("179error\n");
	exit(0);
}

void	set_heredoc_in_process(t_process *process, t_shell_var *shell_var)
{
	t_list				*itr; // token_list;
	t_redirection_kind	kind;
	pid_t				pid;
	int					wstatus;

	itr = process->token_list;
	while (itr)
	{
		if (((t_token *)(itr->content))->kind == TK_REDIRECT)
		{
			kind = get_redirection_kind(((t_token *)(itr->content))->str);
			itr = itr->next;
			if (kind == HEREDOC)
			{
				process->kind[0] = kind;
				process->filename[0] = ((t_token *)(itr->content))->str;
				pipe(process->here_pipefd);
				pid = fork();
				if (pid == 0)
					set_heredoc_sub(process, process->filename[0], shell_var);
				else
				{
					process->here_fd = process->here_pipefd[0];
					close(process->here_pipefd[1]);
					waitpid(pid, &wstatus, WUNTRACED);
				}
			}
		}
		itr = itr->next;
	}
}

void	set_heredoc(t_node *tree, t_shell_var *shell_var)
{
	t_list	*itr;
	pid_t	pid;

	if (ND_SUBSHELL <= tree->kind && tree->kind <= ND_DPIPE)
	{
		if (tree->lhs)
		{
			set_heredoc(tree->lhs, shell_var);
		}
		if (tree->rhs)
		{
			set_heredoc(tree->rhs, shell_var);
		}
	}
	else
	{
		itr = tree->expression->process_list;
		while (itr)
		{
			set_heredoc_in_process((t_process *)itr->content, shell_var);
			itr = itr->next;
		}
	}
}
