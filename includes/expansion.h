#ifndef EXPANSION_H
# define EXPANSION_H

# include <stdlib.h>
# include "libft.h"
# include "struct.h"
# include "parser.h"
# include "shell_var.h"

void			expansion(t_expression *expression, t_shell_var *shell_var);
t_list			*get_expansion_list(char *str, bool par_in_dquote, t_shell_var *shell_var);
t_list			*remove_quotes(t_list *src_list);

#endif
