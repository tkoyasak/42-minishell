#ifndef EXECUTION_H
# define EXECUTION_H

# include "lexer.h"
# include "parser.h"
# include "expansion.h"

typedef enum e_redirection_kind
{
	NONE,
	INPUT,
	HEREDOC,
	OUTPUT,
	APPEND
}	t_redirection_kind;

typedef struct s_process
{
	t_list				*token_list;
	t_redirection_kind	input_kind;
	t_redirection_kind	output_kind;
	int					input_fd;
	int					output_fd;
	char				*input_filename;
	char				*output_filename;
}	t_process;

typedef struct s_expression
{
	t_list					*process_list;
	t_node_kind				end_of_expression; // 最後のprocessはND_SEMICOLON
}	t_expression;

t_list		*convert_to_expression_list(t_node *tree);

#endif
