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
# define PARENTHESIS_CHAR "()"

int		lexer(char *line, t_list **token_list);

#endif
