#include <gtest/gtest.h>

extern "C" {
	#include "lexer.h"
}

void	func(t_token *tokens, char *expected[], t_token_kind expected_kind[])
{
	int	i;

	i = 0;
	while (tokens)
	{
		EXPECT_EQ(tokens->kind, expected_kind[i]);
		EXPECT_STREQ(tokens->str, expected[i]);
		tokens = tokens->next;
		i++;
	}
}

TEST(lexer, lexer_test00)
{
	char *input = (char *)"echo \"hel'lo' w\"'o\"rld'";
	char	*expected[3] = {"echo", "\"hel'lo' w\"'o\"rld'", "\0"};
	t_token_kind expected_kind[3] = {TK_STRING, TK_STRING, TK_EOF};

	t_token *tokens = lexer(1, input);
	func(tokens, expected, expected_kind);
}

TEST(lexer, lexer_test01)
{
	char *input = (char *)"echo \"hel'lo' w\"'o\"rld'";
	char	*expected[3] = {"echo", "\"hel'lo' w\"'o\"rld'", "\0"};
	t_token_kind expected_kind[3] = {TK_STRING, TK_STRING, TK_EOF};

	t_token *tokens = lexer(1, input);
	func(tokens, expected, expected_kind);
}

TEST(lexer, lexer_test02)
{
	char *input = (char *)"cd . ; ls  -l | wc -l";
	char	*expected[] = {"cd", ".", ";", "ls", "-l", "|", "wc", "-l", "\0"};
	t_token_kind expected_kind[] = {TK_STRING, TK_STRING, TK_RESERVED, TK_STRING, TK_STRING, TK_RESERVED, TK_STRING, TK_STRING, TK_EOF};

	t_token *tokens = lexer(1, input);
	func(tokens, expected, expected_kind);
}

TEST(lexer, lexer_test03)
{
	char *input = (char *)"ls -al | cat";
	char	*expected[] = {"ls", "-al", "|", "cat", "\0"};
	t_token_kind expected_kind[] = {TK_STRING, TK_STRING, TK_RESERVED, TK_STRING, TK_EOF};

	t_token *tokens = lexer(1, input);
	func(tokens, expected, expected_kind);
}
