#ifndef SHELL_VAR_H
# define SHELL_VAR_H

# include "libft.h"
# include "struct.h"

t_list		*init_envlist(void);
char		**get_environ(t_shell_var *shell_var);
char		*get_env_value(char *key, t_shell_var *shell_var);
void		set_env_value(char *key, char *val, t_shell_var *shell_var);

#endif
