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
# define PROCESS_DELIM_CHAR "|&;"
# define REDIRECT_CHAR "<>"
# define QUOTE_CHAR "\"\'"
# define PARENTHESIS_CHAR "()"

typedef enum e_token_kind
{
	TK_PROCESS_DELIM, // ; | && ||
	TK_REDIRECT, // < << > >>
	TK_STRING,
	TK_L_PARENTHESIS, // (
	TK_R_PARENTHESIS, // )
}	t_token_kind;

// <, >, <<, >>, ;, &&, ||, |

typedef struct s_token
{
	t_token_kind	kind;
	char			*str;
}	t_token;

t_list		*lexer(char *line);

#endif
