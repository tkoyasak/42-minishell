#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>

# include "libft.h"

# include "lexer.h"
# include "parser.h"
# include "expansion.h"
# include "execution.h"
# include "shell_var.h"
# include "builtin.h"

extern volatile sig_atomic_t	g_exit_status;

#endif
