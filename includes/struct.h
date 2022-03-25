/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoyasak <tkoyasak@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 14:16:13 by jkosaka           #+#    #+#             */
/*   Updated: 2022/03/25 01:39:40 by tkoyasak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_H
# define STRUCT_H

/*  shell var  */
typedef struct s_sh_var
{
	t_list		*env_list;
	char		*pwd;
	char		*oldpwd;
}	t_sh_var;

/*  environment variable  */
typedef struct s_env
{
	char	*key;
	char	*val;
}	t_env;

/*  TK_DELIM is token process delimiter such as ; | && ||
TK_IO is such as <, >, <<, >>  */
typedef enum e_token_kind
{
	TK_DELIM,
	TK_IO,
	TK_STRING,
	TK_L_PAREN,
	TK_R_PAREN,
}	t_token_kind;

typedef struct s_token
{
	t_token_kind	kind;
	char			*str;
}	t_token;

/*  expander kind  */
typedef enum e_expd_kind
{
	PD_SQUOTE,
	PD_DQUOTE,
	PD_ENV,
	PD_STRING,
	PD_NAKED_SP,
	PD_FILENAME,
	PD_HEREDOC
}	t_expd_kind;

/*  expander  */
typedef struct s_expd
{
	int			len;
	char		*str;
	bool		in_squote;
	bool		in_dquote;
	t_expd_kind	kind;
}	t_expd;

/*  input/output kind  */
typedef enum e_io_kind
{
	IO_NONE,
	IO_INPUT,
	IO_HEREDOC,
	IO_OUTPUT,
	IO_APPEND
}	t_io_kind;

/*  process  */
typedef struct s_proc
{
	t_list		*token_list;
	t_io_kind	kind[2];
	int			fd[2];
	char		*filename[2];
	int			here_pipefd[2];
	int			here_fd;
	char		**command;
}	t_proc;

typedef enum e_node_kind
{
	ND_SUBSHELL,
	ND_SEMICOLON,
	ND_DAND,
	ND_DPIPE,
	ND_PIPE,
	ND_PROC,
	ND_EXPR
}	t_node_kind;

/*  expression  */
typedef struct s_expr
{
	t_list		*proc_list;
	int			proc_cnt;
	int			**pipefd;
	pid_t		*pid;
}	t_expr;

typedef struct s_node
{
	t_node_kind		kind;
	t_list			*token_list;
	t_expr			*expr;
	struct s_node	*lhs;
	struct s_node	*rhs;
}	t_node;

#endif
