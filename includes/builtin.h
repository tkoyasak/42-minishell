#ifndef BUILTIN_H
# define BUILTIN_H

# include "execution.h"

int		builtin_echo(t_expression *expression, t_process *process, int cmd_idx);
int		builtin_cd(t_expression *expression, t_process *process, int cmd_idx);
int		builtin_pwd(t_expression *expression, t_process *process, int cmd_idx);
int		builtin_export(t_expression *expression, t_process *process, int cmd_idx);
int		builtin_unset(t_expression *expression, t_process *process, int cmd_idx);
int		builtin_env(t_expression *expression, t_process *process, int cmd_idx);
int		builtin_exit(t_expression *expression, t_process *process, int cmd_idx);

void	write_in_process(t_expression *expression, t_process *process, int cmd_idx, char *str);

#endif
