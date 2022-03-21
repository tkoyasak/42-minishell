#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <errno.h>
# include <sys/stat.h>

# include "libft.h"

# include "lexer.h"
# include "parser.h"
# include "expansion.h"
# include "execution.h"
# include "shell_var.h"
# include "builtin.h"
# include "utils.h"

# define RED "\x1b[31m"
# define GREEN "\x1b[32m"
# define YELLOW "\x1b[33m"
# define BLUE "\x1b[34m"
# define MAGENTA "\x1b[35m"
# define CYAN "\x1b[36m"
# define RESET "\x1b[0m"

# define PROMPT CYAN"minish$ "RESET
# define HEREDOC_PROMPT "> "

extern volatile sig_atomic_t	g_exit_status;

#endif
