#include <gtest/gtest.h>

extern "C" {
	#include "libft.h"
	#include "lexer.h"
}

void	func(t_list *token_list, char *expected[], t_token_kind expected_kind[])
{
	t_list	*itr;
	int	i;

	i = 0;
	itr = token_list;
	while (itr)
	{
		EXPECT_EQ(((t_token *)(itr->content))->kind, expected_kind[i]);
		EXPECT_STREQ(((t_token *)(itr->content))->str, expected[i]);
		i++;
		itr = itr->next;
	}
}

TEST(lexer, lexer_test00)
{
	char *input = "echo \"hel'lo' w\"'o\"rld'";
	char	*expected[] = {"echo", "\"hel'lo' w\"'o\"rld'"};
	t_token_kind expected_kind[] = {TK_STRING, TK_STRING};

	t_list *token_list = lexer(input);
	func(token_list, expected, expected_kind);
}

TEST(lexer, lexer_test01)
{
	char *input = "echo \"hel'lo' w\"'o\"rld'";
	char	*expected[3] = {"echo", "\"hel'lo' w\"'o\"rld'"};
	t_token_kind expected_kind[3] = {TK_STRING, TK_STRING};

	t_list *token_list = lexer(input);
	func(token_list, expected, expected_kind);
}

TEST(lexer, lexer_test02)
{
	char *input = "cd . ; ls  -l | wc -l";
	char	*expected[] = {"cd", ".", ";", "ls", "-l", "|", "wc", "-l"};
	t_token_kind expected_kind[] = {TK_STRING, TK_STRING, TK_PROCESS_DELIM, TK_STRING, TK_STRING, TK_PROCESS_DELIM, TK_STRING, TK_STRING};

	t_list *token_list = lexer(input);
	func(token_list, expected, expected_kind);
}

TEST(lexer, lexer_test03)
{
	char *input = "ls -al | cat";
	char	*expected[] = {"ls", "-al", "|", "cat"};
	t_token_kind expected_kind[] = {TK_STRING, TK_STRING, TK_PROCESS_DELIM, TK_STRING};

	t_list *token_list = lexer(input);
	func(token_list, expected, expected_kind);
}

TEST(lexer, lexer_test04)
{
	char *input = "ls -l |wc -l";
	char	*expected[] = {"ls", "-l", "|", "wc", "-l"};
	t_token_kind expected_kind[] = {TK_STRING, TK_STRING, TK_PROCESS_DELIM, TK_STRING, TK_STRING};

	t_list *token_list = lexer(input);
	func(token_list, expected, expected_kind);
}

TEST(lexer, lexer_test05)
{
	char *input = "ls>>file.txt";
	char	*expected[] = {"ls", ">>", "file.txt"};
	t_token_kind expected_kind[] = {TK_STRING, TK_REDIRECT, TK_STRING};

	t_list *token_list = lexer(input);
	func(token_list, expected, expected_kind);
}

TEST(lexer, lexer_test06)
{
	char *input = "cat \"abc'def\">>file.txt";
	char	*expected[] = {"cat", "\"abc'def\"",">>", "file.txt"};
	t_token_kind expected_kind[] = {TK_STRING, TK_STRING, TK_REDIRECT, TK_STRING};

	t_list *token_list = lexer(input);
	func(token_list, expected, expected_kind);
}

TEST(lexer, lexer_test07)
{
	char *input = "ls|cat>file.txt";
	char	*expected[] = {"ls", "|", "cat", ">", "file.txt"};
	t_token_kind expected_kind[] = {TK_STRING, TK_PROCESS_DELIM, TK_STRING, TK_REDIRECT, TK_STRING};

	t_list *token_list = lexer(input);
	func(token_list, expected, expected_kind);
}

TEST(lexer, lexer_test08)
{
	char *input = "ls;cat>file.txt;";
	char	*expected[] = {"ls", ";", "cat", ">", "file.txt", ";"};
	t_token_kind expected_kind[] = {TK_STRING, TK_PROCESS_DELIM, TK_STRING, TK_REDIRECT, TK_STRING, TK_PROCESS_DELIM};

	t_list *token_list = lexer(input);
	func(token_list, expected, expected_kind);
}

TEST(lexer, lexer_test09)
{
	char *input = "(ls;(cat>file.txt;))";
	char	*expected[] = {"(", "ls", ";", "(", "cat", ">", "file.txt", ";", ")", ")"};
	t_token_kind expected_kind[] = {TK_L_PARENTHESIS, TK_STRING, TK_PROCESS_DELIM, TK_L_PARENTHESIS, TK_STRING, TK_REDIRECT, TK_STRING, TK_PROCESS_DELIM, TK_R_PARENTHESIS, TK_R_PARENTHESIS};

	t_list *token_list = lexer(input);
	func(token_list, expected, expected_kind);
}
