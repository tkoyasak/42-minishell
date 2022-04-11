/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_heredoc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoyasak <tkoyasak@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 11:57:02 by jkosaka           #+#    #+#             */
/*   Updated: 2022/04/10 19:36:00 by tkoyasak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	heredoc_child(t_proc *proc, char *limiter, t_sh_var *sh_var)
{
	char	*temp;
	bool	in_quote;

	safe_func(close(proc->here_pipefd[PIPEIN]));
	in_quote = false;
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

static int	heredoc_parent(t_proc *proc, pid_t pid)
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

static int	set_heredoc_in_token(t_proc *proc, t_sh_var *sh_var)
{
	pid_t	pid;
	int		status;

	safe_func(pipe(proc->here_pipefd));
	pid = safe_func(fork());
	if (pid == 0)
	{
		xsigaction(SIGINT, SIG_DFL);
		heredoc_child(proc, proc->filename[0], sh_var);
	}
	else
	{
		xsigaction(SIGINT, SIG_IGN);
		status = heredoc_parent(proc, pid);
		xsigaction(SIGINT, sigint_handler);
		return (status);
	}
	return (0);
}

static int	set_heredoc_in_proc(t_proc *proc, t_sh_var *sh_var)
{
	t_list		*itr;
	t_io_kind	kind;
	t_token		*token;

	itr = proc->token_list;
	while (itr)
	{
		token = itr->content;
		if (token->kind == TK_IO)
		{
			kind = get_io_kind(token->str);
			itr = itr->next;
			token = itr->content;
			if (kind == IO_HEREDOC)
			{
				proc->kind[0] = kind;
				free(proc->filename[0]);
				proc->filename[0] = ft_xstrdup(token->str);
				if (set_heredoc_in_token(proc, sh_var) == 1)
					return (1);
			}
		}
		itr = itr->next;
	}
	return (0);
}

int	set_heredoc(t_node *tree, t_sh_var *sh_var)
{
	t_list	*itr;
	t_proc	*proc;

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
			proc = itr->content;
			if (set_heredoc_in_proc(proc, sh_var) == 1)
				return (1);
			itr = itr->next;
		}
	}
	return (0);
}
