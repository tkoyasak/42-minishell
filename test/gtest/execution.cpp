#include <gtest/gtest.h>

extern "C" {
	#include "lexer.h"
	#include "parser.h"
	#include "expander.h"
	#include "executor.h"
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

TEST(executor, executor_test00)
{
	char		*input		= "echo aa | cat > outfile";
	char		*input_bash	= "echo aa | cat > outfile_bash";
	t_node		*expr_list = expander(input);

	executor(expr_list);
	func();
}