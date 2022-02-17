#include "lexer.h"
#include "parser.h"


t_node	*parser(t_token *tokens)
{
	t_node	*tree;

	
}

t_node	*parer(char *line)
{
	t_token	*tokens;
	t_node	*tree;

	tokens = lexer(line);
	tree = parser(tokens);

	return (tree);
}
