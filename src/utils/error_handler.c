/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoyasak <tkoyasak@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/25 11:05:53 by tkoyasak          #+#    #+#             */
/*   Updated: 2022/04/07 14:42:10 by tkoyasak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	error_handler(char *str)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	perror(str);
	exit(EXIT_FAILURE);
}

int	fd_error_handler(char *cmd)
{
	int	ret;

	ret = 0;
	if (errno == EBADF)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(cmd, STDERR_FILENO);
		ft_putendl_fd(": write error: Bad file descriptor", STDERR_FILENO);
		ret = 1;
	}
	errno = 0;
	return (ret);
}
