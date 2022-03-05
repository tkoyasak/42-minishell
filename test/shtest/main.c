#include "lexer.h"
#include "parser.h"
#include "expansion.h"
#include "execution.h"
#include "libft.h"

int	main(int argc, char **argv)
{
	if (argc != 2)
		return (1);
	t_node		*expression_list = expansion(argv[1]);

	execution(expression_list);
	return (0);
}
