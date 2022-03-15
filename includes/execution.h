#ifndef EXECUTION_H
# define EXECUTION_H

# include <fcntl.h>
# include "libft.h"
# include "shell_var.h"
# include "expansion.h"

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
int			evaluate_expression(t_expression *expression, t_shell_var *shell_var);
char		*get_fullpath_cmd(char *cmd, t_shell_var *shell_var);
int			execution(t_node *tree, t_shell_var *shell_var);
void		set_heredoc(t_list *expression_list, t_shell_var *shell_var);
void		set_heredoc_sub(t_process *process, char *limiter, t_shell_var *shell_var);
int			exec_builtin(t_expression *expression, t_process *process, t_shell_var *shell_var);
bool		is_builtin(char *cmd);
int			exec_processes(t_expression *expression, t_shell_var *shell_var);
int			exec_single_process(t_expression *expression, t_shell_var *shell_var);
void		set_redirections_and_commands(t_expression *expression, t_shell_var *shell_var);
void		exec_child(t_expression *expression, t_process *process, const int cmd_idx, t_shell_var *shell_var);
t_redirection_kind	get_redirection_kind(char *redirect_str);
void		dup2_func(t_expression *expression, t_process *process, const int cmd_idx);
void		close_func(t_expression *expression, t_process *process, const int cmd_idx);


#endif
