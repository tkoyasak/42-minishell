/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_heredoc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoyasak <tkoyasak@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 11:57:02 by jkosaka           #+#    #+#             */
/*   Updated: 2022/03/25 14:26:24 by tkoyasak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	heredoc_child(t_proc *proc, char *limiter, t_sh_var *sh_var)
{
	char	*temp;
	bool	in_quote;

	safe_func((ssize_t)signal(SIGINT, SIG_DFL));
	safe_func(close(proc->here_pipefd[PIPEIN]));
	limiter = remove_quote_heredoc(limiter, &in_quote);
	temp = readline(HEREDOC_PROMPT);
	while (temp && ft_strcmp(temp, limiter))
	{
		if (in_quote == false)
			temp = expander_heredoc(temp, sh_var);
		ft_putendl_fd(temp, proc->here_pipefd[PIPEOUT]);
		free(temp);
		temp = readline(HEREDOC_PROMPT);
	}
	free(temp);
	free(limiter);
	exit(0);
}

int	heredoc_parent(t_proc *proc, pid_t pid)
{
	int	wstatus;
	int	child_status;

	proc->here_fd = proc->here_pipefd[PIPEIN];
	safe_func(close(proc->here_pipefd[PIPEOUT]));
	waitpid(pid, &wstatus, WUNTRACED);
	if (WIFSIGNALED(wstatus) && (WTERMSIG(wstatus) == SIGINT))
	{
		safe_func(close(proc->here_pipefd[PIPEIN]));
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

int	set_heredoc_in_token(t_proc *proc, t_sh_var *sh_var)
{
	pid_t				pid;

	safe_func((ssize_t)signal(SIGINT, SIG_IGN));
	safe_func(pipe(proc->here_pipefd));
	pid = safe_func(fork());
	if (pid == 0)
		heredoc_child(proc, proc->filename[0], sh_var);
	else
	{
		if (heredoc_parent(proc, pid))
			return (1);
	}
	return (0);
}

// itr  token_list;
int	set_heredoc_in_proc(t_proc *proc, t_sh_var *sh_var)
{
	t_list		*itr;
	t_io_kind	kind;

	itr = proc->token_list;
	while (itr)
	{
		if (((t_token *)(itr->content))->kind == TK_IO)
		{
			kind = get_io_kind(((t_token *)(itr->content))->str);
			itr = itr->next;
			if (kind == IO_HEREDOC)
			{
				proc->kind[0] = kind;
				free(proc->filename[0]);
				proc->filename[0] = ft_xstrdup(((t_token *)(itr->content))->str);
				if (set_heredoc_in_token(proc, sh_var) == 1)
					return (1);
			}
		}
		itr = itr->next;
	}
	safe_func((ssize_t)signal(SIGINT, sigint_handler));
	return (0);
}

int	set_heredoc(t_node *tree, t_sh_var *sh_var)
{
	t_list	*itr;

	if (ND_SUBSHELL <= tree->kind && tree->kind <= ND_DPIPE)
	{
		if (tree->lhs)
			if (set_heredoc(tree->lhs, sh_var) == 1)
				return (1);
		if (tree->rhs)
			if (set_heredoc(tree->rhs, sh_var) == 1)
				return (1);
	}
	else
	{
		itr = tree->expr->proc_list;
		while (itr)
		{
			if (set_heredoc_in_proc((t_proc *)itr->content, sh_var) \
					== 1)
				return (1);
			itr = itr->next;
		}
	}
	return (0);
}
