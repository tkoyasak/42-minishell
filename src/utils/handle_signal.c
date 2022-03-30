/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_signal.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoyasak <tkoyasak@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/25 11:06:00 by tkoyasak          #+#    #+#             */
/*   Updated: 2022/03/30 15:36:36 by tkoyasak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	last_proc_signal(int wstatus)
{
	if (WIFSIGNALED(wstatus))
	{
		if (WTERMSIG(wstatus) == SIGQUIT)
			ft_putendl_fd("Quit: 3", STDERR_FILENO);
		else if (WTERMSIG(wstatus) == SIGINT)
			ft_putchar_fd('\n', STDERR_FILENO);
		g_exit_status = WTERMSIG(wstatus) + 128;
	}
	else
		g_exit_status = WEXITSTATUS(wstatus);
}

void	sigint_handler(int sig)
{
	(void)sig;
	g_exit_status = 1;
	ft_putchar_fd('\n', STDOUT_FILENO);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	xsignal(int sig, void (*handler)(int))
{
	if (signal(sig, handler) == SIG_ERR)
		error_handler("signal");
}
