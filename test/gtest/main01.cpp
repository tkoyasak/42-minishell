#include <gtest/gtest.h>

extern "C" {
	#include "lexer.h"
	#include "parser.h"
}

int token_idx;
int	node_idx;

void	dfs(t_node *node, char *expected_token[], t_node_kind expected_node[])
{
	t_list	*itr;

	if (node->lhs)
		dfs(node->lhs, expected_token, expected_node);
	EXPECT_EQ(node->kind, expected_node[node_idx]);
	node_idx++;
	if (node->kind != ND_PROC)
		token_idx++;
	itr = node->token_list;
	while (itr)
	{
		EXPECT_STREQ(((t_token *)(itr->content))->str, expected_token[token_idx]);
		itr = itr->next;
		token_idx++;
	}
	if (node->rhs)
		dfs(node->rhs, expected_token, expected_node);
}

void	func(t_node *node, char *expected_token[], t_node_kind expected_node[])
{
	token_idx = 0;
	node_idx = 0;
	dfs(node, expected_token, expected_node);
}

TEST(parser, parser_test00)
{
	char		*input = "ls -al | cat";
	char		*expected_token[] = {"ls", "-al", "|", "cat"};
	t_node_kind expected_node[] = {ND_PROC, ND_PIPE, ND_PROC};
	t_node		*tree = parser(input);

	func(tree, expected_token, expected_node);
}

TEST(parser, parser_test01)
{
	char		*input = "ls -al|cat|echo \"hello\">>file";
	char		*expected_token[] = {"ls", "-al", "|", "cat", "|", "echo", "\"hello\"", ">>", "file"};
	t_node_kind expected_node[] = {ND_PROC, ND_PIPE, ND_PROC, ND_PIPE, ND_PROC};
	t_node		*tree = parser(input);

	func(tree, expected_token, expected_node);
}

TEST(parser, parser_test02)
{
	char		*input = "ls -al|cat;cat>file";
	char		*expected_token[] = \
		{"ls", "-al", "|", "cat", ";", "cat", ">", "file"};
	t_node_kind expected_node[] = \
		{ND_PROC, ND_PIPE, ND_PROC, ND_SEMICOLON, ND_PROC};
	t_node		*tree = parser(input);

	func(tree, expected_token, expected_node);
}

TEST(parser, parser_test03)
{
	char		*input = "ls -al;cd;echo \"hello w\"'orld';cat>file";
	char		*expected_token[] = \
		{"ls", "-al", ";", "cd", ";", "echo", "\"hello w\"'orld'", ";", "cat", ">", "file"};
	t_node_kind expected_node[] = \
		{ND_PROC, ND_SEMICOLON, ND_PROC, ND_SEMICOLON, ND_PROC, ND_SEMICOLON, ND_PROC};
	t_node		*tree = parser(input);

	func(tree, expected_token, expected_node);
}

TEST(parser, parser_test04)
{
	char		*input = "ls -al ; cat | cat>file;cat>file";
	char		*expected_token[] = \
		{"ls", "-al", ";", "cat", "|", "cat", ">", "file", ";", "cat", ">", "file"};
	t_node_kind expected_node[] = \
		{ND_PROC, ND_SEMICOLON, ND_PROC, ND_PIPE, ND_PROC, ND_SEMICOLON, ND_PROC};
	t_node		*tree = parser(input);

	func(tree, expected_token, expected_node);
}

TEST(parser, parser_test05)
{
	char		*input = "ls -al&&cd&&cat>file;cat>file";
	char		*expected_token[] = \
		{"ls", "-al", "&&", "cd", "&&", "cat", ">", "file", ";", "cat", ">", "file"};
	t_node_kind expected_node[] = \
		{ND_PROC, ND_DAND, ND_PROC, ND_DAND, ND_PROC, ND_SEMICOLON, ND_PROC};
	t_node		*tree = parser(input);

	func(tree, expected_token, expected_node);
}

TEST(parser, parser_test06)
{
	char		*input = "ls -al ; cat || cat>file||cat>file";
	char		*expected_token[] = \
		{"ls", "-al", ";", "cat", "||", "cat", ">", "file", "||", "cat", ">", "file"};
	t_node_kind expected_node[] = \
		{ND_PROC, ND_SEMICOLON, ND_PROC, ND_DPIPE, ND_PROC, ND_DPIPE, ND_PROC};
	t_node		*tree = parser(input);

	func(tree, expected_token, expected_node);
}

TEST(parser, parser_test07)
{
	char		*input = "ls -al ; cat ;";
	char		*expected_token[] = \
		{"ls", "-al", ";", "cat", ";"};
	t_node_kind expected_node[] = \
		{ND_PROC, ND_SEMICOLON, ND_PROC, ND_SEMICOLON};
	t_node		*tree = parser(input);

	func(tree, expected_token, expected_node);
}

TEST(parser, parser_test08)
{
	char		*input = "ls -al ; cat ; cd . ; ls -a ;";
	char		*expected_token[] = \
		{"ls", "-al", ";", "cat", ";", "cd", ".", ";", "ls", "-a", ";"};
	t_node_kind expected_node[] = \
		{ND_PROC, ND_SEMICOLON, ND_PROC, ND_SEMICOLON, ND_PROC, ND_SEMICOLON, ND_PROC, ND_SEMICOLON};
	t_node		*tree = parser(input);

	func(tree, expected_token, expected_node);
}

TEST(parser, parser_test09)
{
	char		*input = "(ls -al  && cd)";
	char		*expected_token[] = \
		{"ls", "-al", "&&", "cd", "subshell"};
	t_node_kind expected_node[] = \
		{ND_PROC, ND_DAND, ND_PROC, ND_SUBSHELL};
	t_node		*tree = parser(input);

	func(tree, expected_token, expected_node);
}
