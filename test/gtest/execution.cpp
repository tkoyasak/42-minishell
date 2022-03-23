#include <gtest/gtest.h>

extern "C" {
	#include "lexer.h"
	#include "parser.h"
	#include "expansion.h"
	#include "execution.h"
	#include "libft.h"
	#include <stdlib.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <unistd.h>
}

extern char	**environ;

void	func()
{
	EXPECT_EQ(system("diff outfile outfile_bash"), 0);
}

TEST(execution, execution_test00)
{
	char		*input		= "echo aa | cat > outfile";
	char		*input_bash	= "echo aa | cat > outfile_bash";
	t_node		*expr_list = expansion(input);

	execution(expr_list);
	func();
}