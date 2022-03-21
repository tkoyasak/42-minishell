#ifndef EXECUTION_H
# define EXECUTION_H

# include <fcntl.h>
# include "libft.h"
# include "struct.h"
# include "shell_var.h"
# include "expansion.h"

# define PIPEIN 0
# define PIPEOUT 1
# define NOFILE -2
# define NOCMD 127
# define FD_MAX 256
# define PATH_PREFIX 5

t_node		*convert_to_expression_tree(t_node *tree);
int			evaluate_expression(t_expression *expression, t_shell_var *shell_var);
char		*get_fullpath_cmd(char *cmd, t_shell_var *shell_var);
int			execution(t_node *tree, t_shell_var *shell_var);
int			set_heredoc(t_node *tree, t_shell_var *shell_var);
void		set_heredoc_sub(t_process *process, char *limiter, t_shell_var *shell_var);
size_t		get_word_len_heredoc(char *str);
t_list		*extract_word_heredoc(char **str, bool in_squote, bool in_dquote, t_expansion_kind kind);
t_list		*split_str_heredoc(char *str, bool par_in_dquote);
t_list		*get_expansion_list_heredoc(char *str, bool par_in_dquote, t_shell_var *shell_var);
size_t		get_expanded_len_heredoc(t_list *exp_list);
char		*expansion_heredoc(char *str, t_shell_var *shell_var);
char		*remove_quote_heredoc(char *limiter, bool *in_quote);

int			exec_builtin(t_process *process, t_shell_var *shell_var);
bool		is_builtin(char *cmd);
int			exec_processes(t_expression *expression, t_shell_var *shell_var);
int			exec_single_process(t_expression *expression, t_shell_var *shell_var);
void		set_redirections_and_commands(t_expression *expression, t_shell_var *shell_var);
void		exec_child(t_expression *expression, t_process *process, const int cmd_idx, t_shell_var *shell_var);
t_redirection_kind	get_redirection_kind(char *redirect_str);
void		dup2_func(t_expression *expression, t_process *process, const int cmd_idx);
void		close_func(t_expression *expression, t_process *process, const int cmd_idx);

#endif
