#ifndef EXPANSION_H
# define EXPANSION_H

# include <stdlib.h>

# include "parser.h"
# include "libft.h"

typedef enum e_exp_strlist_type
{
	SQUOTE,
	DQUOTE,
	ENV,
	STRING,
	STRING_SPACE,
	NAKED_SPACE
}				t_exp_strlist_type;

typedef struct s_exp_strlist
{
	int						len;
	char					*str;
	bool					in_squote;
	bool					in_dquote;
	struct s_exp_strlist	*next;
}	t_exp_strlist;

t_node	*expansion(char *argv);

#endif
