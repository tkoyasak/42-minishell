#ifndef LEXER_H
# define LEXER_H

# include <stdlib.h>
# include <stdbool.h>
# include <stdio.h>
# include <string.h>
# include <ctype.h>

typedef enum e_token_kind
{
	TK_RESERVED,
	TK_STRING,
	TK_EOF
}	t_token_kind;

// <, >, <<, >>, ;, &&, ||, |

typedef struct s_token
{
	t_token_kind	kind;
	struct s_token	*next;
	char			*str;
}	t_token;

t_token		*lexer(int argc, char *argv);

#endif
