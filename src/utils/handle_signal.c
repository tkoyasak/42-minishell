/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_signal.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoyasak <tkoyasak@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/25 11:06:00 by tkoyasak          #+#    #+#             */
/*   Updated: 2022/04/11 20:16:51 by tkoyasak         ###   ########.fr       */
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
	if (sig == SIGINT)
		g_exit_status = 1;
}

void	xsigaction(int sig, void (*handler)(int))
{
	struct sigaction	sa;

	sa.sa_handler = handler;
	sa.sa_flags = 0;
	if (sigemptyset(&sa.sa_mask) == -1)
		error_handler("sigemptyset");
	if (sigaddset(&sa.sa_mask, sig) == -1)
		error_handler("sigaddset");
	if (sigaction(sig, &sa, NULL) == -1)
		error_handler("sigaction");
}

static int	rl_signal_hook(void)
{
	ft_putchar_fd('\n', STDOUT_FILENO);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	return (0);
}

void	install_signal_handle(void)
{
	xsigaction(SIGINT, sigint_handler);
	xsigaction(SIGQUIT, SIG_IGN);
	rl_signal_event_hook = rl_signal_hook;
}
