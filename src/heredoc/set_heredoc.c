#include "minishell.h"

int	heredoc_child(t_process *process, char *limiter, t_shell_var *shell_var)
{
	size_t	len;
	char	*temp;
	bool	in_quote;

	safe_func((ssize_t)signal(SIGINT, SIG_DFL));
	safe_func(close(process->here_pipefd[0]));
	limiter = remove_quote_heredoc(limiter, &in_quote);
	len = ft_strlen(limiter);
	limiter[len] = '\0';
	temp = readline(HEREDOC_PROMPT);
	while (temp && ft_strncmp(temp, limiter, len + 1))
	{
		if (in_quote == false)
			temp = expansion_heredoc(temp, shell_var);
		ft_putendl_fd(temp, process->here_pipefd[1]);
		free(temp);
		temp = readline(HEREDOC_PROMPT);
	}
	free(temp);
	free(limiter);
	exit(0);
}

int	heredoc_parent(t_process *process, pid_t pid)
{
	int	wstatus;
	int	child_status;

	process->here_fd = process->here_pipefd[0];
	safe_func(close(process->here_pipefd[1]));
	waitpid(pid, &wstatus, WUNTRACED);
	if (WIFSIGNALED(wstatus) && (WTERMSIG(wstatus) == SIGINT))
	{
		safe_func(close(process->here_pipefd[0]));
		ft_putchar_fd('\n', STDOUT_FILENO);
		g_exit_status = 1;
		return (1);
	}
	else
	{
		child_status = WEXITSTATUS(wstatus);
		if (child_status == 1)
		{
			g_exit_status = 1;
			return (1);
		}
		else
			return (0);
	}
}

int	set_heredoc_in_token(t_process *process, t_shell_var *shell_var)
{
	pid_t				pid;

	safe_func((ssize_t)signal(SIGINT, SIG_IGN));
	safe_func(pipe(process->here_pipefd));
	pid = safe_func(fork());
	if (pid == 0)
		heredoc_child(process, process->filename[0], shell_var);
	else
	{
		if (heredoc_parent(process, pid))
			return (1);
	}
	return (0);
}

// itr  token_list;
int	set_heredoc_in_process(t_process *process, t_shell_var *shell_var)
{
	t_list				*itr;
	t_io_kind	kind;

	itr = process->token_list;
	while (itr)
	{
		if (((t_token *)(itr->content))->kind == TK_IO)
		{
			kind = get_io_kind(((t_token *)(itr->content))->str);
			itr = itr->next;
			if (kind == IO_HEREDOC)
			{
				process->kind[0] = kind;
				process->filename[0] = ((t_token *)(itr->content))->str;
				if (set_heredoc_in_token(process, shell_var) == 1)
					return (1);
			}
		}
		itr = itr->next;
	}
	safe_func((ssize_t)signal(SIGINT, sigint_handler));
	return (0);
}

int	set_heredoc(t_node *tree, t_shell_var *shell_var)
{
	t_list	*itr;

	if (ND_SUBSHELL <= tree->kind && tree->kind <= ND_DPIPE)
	{
		if (tree->lhs)
			if (set_heredoc(tree->lhs, shell_var) == 1)
				return (1);
		if (tree->rhs)
			if (set_heredoc(tree->rhs, shell_var) == 1)
				return (1);
	}
	else
	{
		itr = tree->expr->process_list;
		while (itr)
		{
			if (set_heredoc_in_process((t_process *)itr->content, shell_var) \
					== 1)
				return (1);
			itr = itr->next;
		}
	}
	return (0);
}
