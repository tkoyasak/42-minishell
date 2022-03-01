#ifndef LEXER_H
# define LEXER_H

# include <stdlib.h>
# include <stdbool.h>
# include <stdio.h>
# include <string.h>
# include <ctype.h>
# include <assert.h>
# include "libft.h"

# define RESERVED_CHAR "<>|&;"

typedef enum e_token_kind
{
	TK_RESERVED, // ; | && ||
	TK_REDIRECT, // < << > >>
	TK_STRING
}	t_token_kind;

// <, >, <<, >>, ;, &&, ||, |

typedef struct s_token
{
	t_token_kind	kind;
	char			*str;
}	t_token;

t_list		*lexer(char *line);

#endif
