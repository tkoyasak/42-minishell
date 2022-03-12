#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"

# include "lexer.h"
# include "parser.h"
# include "expansion.h"
# include "execution.h"
# include "env.h"
# include "builtin.h"

typedef struct s_shell_var
{
	t_list		*env_list;
	char		*pwd;
	char		*oldpwd;
}	t_shell_var;

#endif