#ifndef EXECUTION_H
# define EXECUTION_H

# include "lexer.h"
# include "parser.h"
# include "expansion.h"

typedef struct s_process
{
	t_token					*token;
}	t_process;

typedef struct s_expression
{
	t_list					*process;
	t_node_kind				end_of_expression; // 最後のprocessはND_SEMICOLON
}	t_expression;

#endif
