/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoyasak <tkoyasak@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/25 11:05:53 by tkoyasak          #+#    #+#             */
/*   Updated: 2022/03/25 11:05:54 by tkoyasak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	error_handler(char *str)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	perror(str);
	exit(EXIT_FAILURE);
}
