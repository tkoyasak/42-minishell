#ifndef LEXER_H
# define LEXER_H

# include <stdlib.h>
# include <stdbool.h>
# include <stdio.h>
# include <string.h>
# include <ctype.h>
# include <assert.h>
# include "libft.h"
# include "struct.h"

# define RESERVED_CHAR "<>|&;"
# define PROCESS_DELIM_CHAR "|&;"
# define REDIRECT_CHAR "<>"
# define QUOTE_CHAR "\"\'"
# define PAREN_CHAR "()"

int		lexer(char *line, t_list **token_list);
bool	set_token_len_and_kind(char *p, int *len, t_token_kind *kind);
void	lexer_error(char *p, int idx);

#endif
