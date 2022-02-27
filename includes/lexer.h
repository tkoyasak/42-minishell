#ifndef LEXER_H
# define LEXER_H

# include <stdlib.h>
# include <stdbool.h>
# include <stdio.h>
# include <string.h>
# include <ctype.h>
# include <assert.h>

# define RESERVED_CHAR "<>|&;"

typedef enum e_token_kind
{
	TK_RESERVED, // ; | && ||
	TK_REDIRECT, // < << > >>
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

t_token		*lexer(char *line);
t_token		*tk_token_new(t_token_kind kind, char *str);
t_token		*tk_token_last(t_token *token);
void		tk_token_add_back(t_token **head, t_token *newsrc);

#endif
