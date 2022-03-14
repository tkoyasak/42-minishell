#ifndef SHELL_VAR_H
# define SHELL_VAR_H

# include "libft.h"

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

t_list		*init_envlist(void);
char		**get_environ(t_shell_var *shell_var);
char		*get_env_value(char *key, t_shell_var *shell_var);

#endif
