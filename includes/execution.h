#ifndef EXECUTION_H
# define EXECUTION_H

# include <fcntl.h>
# include "lexer.h"
# include "parser.h"
# include "expansion.h"

# define STDIN 0
# define STDOUT 1
# define STDERR 2
# define PIPEIN 0
# define PIPEOUT 1
# define NOFILE -2
# define NOCMD 127
# define FD_MAX 256
# define PATH_PREFIX 5

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
	char				*heredoc;
	int					*here_pipefd;
	char				**command;
}	t_process;

typedef struct s_expression
{
	t_list					*process_list;
	int						process_cnt;
	int						**pipefd;
	pid_t					*pid; 
	t_node_kind				end_of_expression; // 最後のprocessはND_SEMICOLON
}	t_expression;

t_list		*convert_to_expression_list(t_node *tree);
int			evaluate_expression(t_expression *expression);
char		*get_fullpath_cmd(char *cmd);
int			execution(t_node *tree);
void		set_heredoc(t_process *process, char *limiter);

#endif
