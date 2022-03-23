#ifndef STRUCT_H
# define STRUCT_H

typedef struct s_shell_var
{
	t_list		*env_list;
	char		*pwd;
	char		*oldpwd;
}	t_shell_var;

typedef struct s_env
{
	char			*key;
	char			*val;
}	t_env;

/*  TK_DELIM is token process delimiter ; | && ||
TK_REDIRECT <, >, <<, >>  */
typedef enum e_token_kind
{
	TK_DELIM,
	TK_REDIRECT,
	TK_STRING,
	TK_L_PAREN,
	TK_R_PAREN,
}	t_token_kind;

typedef struct s_token
{
	t_token_kind	kind;
	char			*str;
}	t_token;


typedef enum e_expansion_kind
{
	SQUOTE,
	DQUOTE,
	ENV,
	STRING,
	NAKED_SPACE,
	FILENAME_EXPANSION
}	t_expansion_kind;

typedef struct s_expansion
{
	int						len;
	char					*str;
	bool					in_squote;
	bool					in_dquote;
	t_expansion_kind		kind;
}	t_expansion;

typedef enum e_redirection_kind
{
	NONE,
	INPUT,
	HEREDOC,
	OUTPUT,
	APPEND
}	t_redirection_kind;

typedef struct s_process
{
	t_list				*token_list;
	t_redirection_kind	kind[2];
	int					fd[2];
	char				*filename[2];
	int					here_pipefd[2];
	int					here_fd;
	char				**command;
}	t_process;

typedef enum e_node_kind
{
	ND_SUBSHELL,
	ND_SEMICOLON, 
	ND_DAND,
	ND_DPIPE,
	ND_PIPE,
	ND_PROCESS,
	ND_EXPRESSION
}	t_node_kind;

typedef struct s_expression
{
	t_list					*process_list;
	int						process_cnt;
	int						**pipefd;
	pid_t					*pid;
}	t_expression;

// 抽象構文木のノードの型
typedef struct s_node
{
	t_node_kind		kind;
	t_list			*token_list;
	t_expression	*expression;
	struct s_node	*lhs;
	struct s_node	*rhs;
}	t_node;

#endif
