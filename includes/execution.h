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
# define NO_CMD 127
# define NO_PERMISSION 126
# define EXIT_OUT_RANGE 255
# define FD_MAX 256
# define PATH_PREFIX 5

/*  execution  */
t_node		*convert_to_expr_tree(t_node *tree);
int			evaluate_expr(t_expr *expr, t_shell_var *shell_var);
char		*get_fullpath_cmd(char *cmd, t_shell_var *shell_var);
int			execution(t_node *tree, t_shell_var *shell_var);
int			set_heredoc(t_node *tree, t_shell_var *shell_var);
int			exec_builtin(t_proc *proc, t_shell_var *shell_var);
bool		is_builtin(char *cmd);
int			exec_procs(t_expr *expr, t_shell_var *shell_var);
int			exec_single_proc(t_expr *expr, t_shell_var *shell_var);
void		exec_child(t_expr *expr, t_proc *proc, const int cmd_idx, t_shell_var *shell_var);
t_io_kind	get_io_kind(char *redirect_str);
void		dup2_func(t_expr *expr, t_proc *proc, const int cmd_idx);
void		close_func(t_expr *expr, t_proc *proc, const int cmd_idx);

/*  io  */
t_io_kind	get_io_kind(char *redirect_str);
void		remove_io_token(t_proc *proc);
void		open_error_handler(char *filename);
void		set_io_params(t_proc *proc);
void		set_command(t_proc *proc);
void		set_io_and_commands(t_expr *expr);

/*  heredoc  */
size_t		get_word_len_heredoc(char *str, bool in_squote, bool in_dquote);
t_list		*extract_word_heredoc(char **str, bool in_squote, bool in_dquote, t_expd_kind kind);
t_list		*split_str_heredoc(char *str, bool par_in_dquote);
t_list		*get_expansion_list_heredoc(char *str, bool par_in_dquote, t_shell_var *shell_var);
size_t		get_expanded_len_heredoc(t_list *exp_list);
char		*expansion_heredoc(char *str, t_shell_var *shell_var);
char		*remove_quote_heredoc(char *limiter, bool *in_quote);


#endif
