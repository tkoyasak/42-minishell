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
	char *input = "echo \"hel'lo' w\"'o\"rld'";
	char	*expected[] = {"echo", "\"hel'lo' w\"'o\"rld'", "\0"};
	t_token_kind expected_kind[] = {TK_STRING, TK_STRING, TK_EOF};

	t_token *tokens = lexer(input);
	func(tokens, expected, expected_kind);
}

TEST(lexer, lexer_test01)
{
	char *input = "echo \"hel'lo' w\"'o\"rld'";
	char	*expected[3] = {"echo", "\"hel'lo' w\"'o\"rld'", "\0"};
	t_token_kind expected_kind[3] = {TK_STRING, TK_STRING, TK_EOF};

	t_token *tokens = lexer(input);
	func(tokens, expected, expected_kind);
}

TEST(lexer, lexer_test02)
{
	char *input = "cd . ; ls  -l | wc -l";
	char	*expected[] = {"cd", ".", ";", "ls", "-l", "|", "wc", "-l", "\0"};
	t_token_kind expected_kind[] = {TK_STRING, TK_STRING, TK_RESERVED, TK_STRING, TK_STRING, TK_RESERVED, TK_STRING, TK_STRING, TK_EOF};

	t_token *tokens = lexer(input);
	func(tokens, expected, expected_kind);
}

TEST(lexer, lexer_test03)
{
	char *input = "ls -al | cat";
	char	*expected[] = {"ls", "-al", "|", "cat", "\0"};
	t_token_kind expected_kind[] = {TK_STRING, TK_STRING, TK_RESERVED, TK_STRING, TK_EOF};

	t_token *tokens = lexer(input);
	func(tokens, expected, expected_kind);
}

TEST(lexer, lexer_test04)
{
	char *input = "ls -l |wc -l";
	char	*expected[] = {"ls", "-l", "|", "wc", "-l", "\0"};
	t_token_kind expected_kind[] = {TK_STRING, TK_STRING, TK_RESERVED, TK_STRING, TK_STRING, TK_EOF};

	t_token *tokens = lexer(input);
	func(tokens, expected, expected_kind);
}

TEST(lexer, lexer_test05)
{
	char *input = "ls>>file.txt";
	char	*expected[] = {"ls", ">>", "file.txt", "\0"};
	t_token_kind expected_kind[] = {TK_STRING, TK_REDIRECT, TK_STRING, TK_EOF};

	t_token *tokens = lexer(input);
	func(tokens, expected, expected_kind);
}

TEST(lexer, lexer_test06)
{
	char *input = "cat \"abc'def\">>file.txt";
	char	*expected[] = {"cat", "\"abc'def\"",">>", "file.txt", "\0"};
	t_token_kind expected_kind[] = {TK_STRING, TK_STRING, TK_REDIRECT, TK_STRING, TK_EOF};

	t_token *tokens = lexer(input);
	func(tokens, expected, expected_kind);
}

TEST(lexer, lexer_test07)
{
	char *input = "ls|cat>file.txt";
	char	*expected[] = {"ls", "|", "cat", ">", "file.txt", "\0"};
	t_token_kind expected_kind[] = {TK_STRING, TK_RESERVED, TK_STRING, TK_REDIRECT, TK_STRING, TK_EOF};

	t_token *tokens = lexer(input);
	func(tokens, expected, expected_kind);
}

TEST(lexer, lexer_test08)
{
	char *input = "ls;cat>file.txt;";
	char	*expected[] = {"ls", ";", "cat", ">", "file.txt", ";", "\0"};
	t_token_kind expected_kind[] = {TK_STRING, TK_RESERVED, TK_STRING, TK_REDIRECT, TK_STRING, TK_RESERVED, TK_EOF};

	t_token *tokens = lexer(input);
	func(tokens, expected, expected_kind);
}
