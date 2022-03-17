#ifndef BUILTIN_H
# define BUILTIN_H

# include "struct.h"
# include "execution.h"

int		builtin_echo(t_expression *expression, t_process *process, t_shell_var *shell_var);
int		builtin_cd(t_expression *expression, t_process *process, t_shell_var *shell_var);
int		builtin_pwd(t_expression *expression, t_process *process, t_shell_var *shell_var);
int		builtin_export(t_expression *expression, t_process *process, t_shell_var *shell_var);
int		builtin_unset(t_expression *expression, t_process *process, t_shell_var *shell_var);
int		builtin_env(t_expression *expression, t_process *process, t_shell_var *shell_var);
int		builtin_exit(t_expression *expression, t_process *process, t_shell_var *shell_var);

#endif
