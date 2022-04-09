/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoyasak <tkoyasak@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 14:04:40 by jkosaka           #+#    #+#             */
/*   Updated: 2022/03/25 12:01:42 by tkoyasak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include <fcntl.h>
# include "libft.h"
# include "struct.h"
# include "shell_var.h"

# define PIPEIN 0
# define PIPEOUT 1
# define NOFILE -2
# define NO_CMD 127
# define NO_PERMISSION 126
# define EXIT_OUT_RANGE 255
# define FD_MAX 256

/*  executor  */
char		*get_cmd_path(char *cmd, t_sh_var *sh_var);
void		executor(t_node *tree, t_sh_var *sh_var);
int			set_heredoc(t_node *tree, t_sh_var *sh_var);
int			exec_builtin(t_proc *proc, t_sh_var *sh_var);
bool		is_builtin(char *cmd);
char		*first_command(t_list *token_list);
int			exec_procs(t_expr *expr, t_sh_var *sh_var);
// int			exec_single_proc(t_expr *expr, t_sh_var *sh_var);
int			exec_single_builtin(t_expr *expr, t_sh_var *sh_var);
void		exec_child(t_expr *expr, t_proc *proc, \
						int cmd_idx, t_sh_var *sh_var);
void		dup2_func(t_expr *expr, t_proc *proc, const int cmd_idx);
void		close_func(t_expr *expr, t_proc *proc, const int cmd_idx);
int			path_error(char *fullpath_cmd);

/*  io  */
t_io_kind	get_io_kind(char *redirect_str);
int			set_io_params(t_proc *proc, t_sh_var *sh_var);
int			set_io_and_commands(t_proc *proc, t_sh_var *sh_var);
void		set_command(t_proc *proc);

/*  heredoc  */
char		*expander_heredoc(char *str, t_sh_var *sh_var);
char		*remove_quote_heredoc(char *limiter, bool *in_quote);

#endif
