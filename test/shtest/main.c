#include "lexer.h"
#include "parser.h"
#include "executor.h"
#include "libft.h"

int	main(int argc, char **argv)
{
	if (argc != 2)
		return (1);
	t_node		*expr_list = expander(argv[1]);

	executor(expr_list);
	return (0);
}
