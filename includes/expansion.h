#ifndef EXPANSION_H
# define EXPANSION_H

# include <stdlib.h>

# include "libft.h"
# include "parser.h"
# include "shell_var.h"

typedef enum e_expansion_kind
{
	SQUOTE,
	DQUOTE,
	ENV,
	STRING,
	NAKED_SPACE
}	t_expansion_kind;

typedef struct s_expansion
{
	int						len;
	char					*str;
	bool					in_squote;
	bool					in_dquote;
	t_expansion_kind		kind;
}	t_expansion;

t_node			*expansion(char *argv, t_shell_var *shell_var);
t_list			*get_expansion_list(char *str, bool par_in_dquote, t_shell_var *shell_var);
t_list			*remove_quotes(t_list *src_list);

#endif
