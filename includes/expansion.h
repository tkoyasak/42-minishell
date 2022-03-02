#ifndef EXPANSION_H
# define EXPANSION_H

# include <stdlib.h>

# include "parser.h"
# include "libft.h"

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

t_node			*expansion(char *argv);
t_list			*get_expansion_list(char *str, bool par_in_dquote);
t_list			*remove_quotes(t_list *src_list);

#endif
