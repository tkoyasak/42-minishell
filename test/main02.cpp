#include <gtest/gtest.h>

extern "C" {
	#include "lexer.h"
	#include "parser.h"
	#include "expansion.h"
}

int token_idx;
int	node_idx;

void	dfs(t_node *node, char *expected_token[], t_node_kind expected_node[])
{
	if (node->lhs)
		dfs(node->lhs, expected_token, expected_node);
	EXPECT_EQ(node->kind, expected_node[node_idx]);
	node_idx++;
	if (node->kind != ND_COMMAND)
		token_idx++;
	while (node->token)
	{
		EXPECT_STREQ(node->token->str, expected_token[token_idx]);
		node->token = node->token->next;
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

TEST(parser, expansion_test00)
{
	char		*input = "ls -al | cat $PATH";
	char		*expected_token[] = {"ls", "-al", "|", "cat", "/Users/koyasako/.nvm/versions/node/v17.4.0/bin:/usr/local/opt/mysql@5.6/bin:/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin:/usr/local/go/bin:/opt/X11/bin:/Library/Apple/usr/bin:/Users/koyasako/.nvm/versions/node/v17.4.0/bin:/usr/local/opt/mysql@5.6/bin:/Users/koyasako/.cargo/bin"};
	t_node_kind expected_node[] = {ND_COMMAND, ND_PIPE, ND_COMMAND};
	t_node		*tree = expansion(input);

	func(tree, expected_token, expected_node);
}

TEST(parser, expansion_test01)
{
	char		*input = "ls -al | cat aa$PATH";
	char		*expected_token[] = {"ls", "-al", "|", "cat", "a/Users/koyasako/.nvm/versions/node/v17.4.0/bin:/usr/local/opt/mysql@5.6/bin:/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin:/usr/local/go/bin:/opt/X11/bin:/Library/Apple/usr/bin:/Users/koyasako/.nvm/versions/node/v17.4.0/bin:/usr/local/opt/mysql@5.6/bin:/Users/koyasako/.cargo/bin"};
	t_node_kind expected_node[] = {ND_COMMAND, ND_PIPE, ND_COMMAND};
	t_node		*tree = expansion(input);

	func(tree, expected_token, expected_node);
}

TEST(parser, expansion_test02)
{
	char		*input = "ls -al | $PATH";
	char		*expected_token[] = {"ls", "-al", "|", "/Users/koyasako/.nvm/versions/node/v17.4.0/bin:/usr/local/opt/mysql@5.6/bin:/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin:/usr/local/go/bin:/opt/X11/bin:/Library/Apple/usr/bin:/Users/koyasako/.nvm/versions/node/v17.4.0/bin:/usr/local/opt/mysql@5.6/bin:/Users/koyasako/.cargo/bin"};
	t_node_kind expected_node[] = {ND_COMMAND, ND_PIPE, ND_COMMAND};
	t_node		*tree = expansion(input);

	func(tree, expected_token, expected_node);
}

// TEST(parser, parser_test01)
// {
// 	char		*input = "ls -al|cat|echo \"hello\">>file";
// 	char		*expected_token[] = {"ls", "-al", "|", "cat", "|", "echo", "\"hello\"", ">>", "file"};
// 	t_node_kind expected_node[] = {ND_COMMAND, ND_PIPE, ND_COMMAND, ND_PIPE, ND_COMMAND};
// 	t_node		*tree = parser(input);

// 	func(tree, expected_token, expected_node);
// }

// TEST(parser, parser_test02)
// {
// 	char		*input = "ls -al|cat;cat>file";
// 	char		*expected_token[] = \
// 		{"ls", "-al", "|", "cat", ";", "cat", ">", "file"};
// 	t_node_kind expected_node[] = \
// 		{ND_COMMAND, ND_PIPE, ND_COMMAND, ND_SEMICOLON, ND_COMMAND};
// 	t_node		*tree = parser(input);

// 	func(tree, expected_token, expected_node);
// }

// TEST(parser, parser_test03)
// {
// 	char		*input = "ls -al;cd;echo \"hello w\"'orld';cat>file";
// 	char		*expected_token[] = \
// 		{"ls", "-al", ";", "cd", ";", "echo", "\"hello w\"'orld'", ";", "cat", ">", "file"};
// 	t_node_kind expected_node[] = \
// 		{ND_COMMAND, ND_SEMICOLON, ND_COMMAND, ND_SEMICOLON, ND_COMMAND, ND_SEMICOLON, ND_COMMAND};
// 	t_node		*tree = parser(input);

// 	func(tree, expected_token, expected_node);
// }

// TEST(parser, parser_test04)
// {
// 	char		*input = "ls -al ; cat | cat>file;cat>file";
// 	char		*expected_token[] = \
// 		{"ls", "-al", ";", "cat", "|", "cat", ">", "file", ";", "cat", ">", "file"};
// 	t_node_kind expected_node[] = \
// 		{ND_COMMAND, ND_SEMICOLON, ND_COMMAND, ND_PIPE, ND_COMMAND, ND_SEMICOLON, ND_COMMAND};
// 	t_node		*tree = parser(input);

// 	func(tree, expected_token, expected_node);
// }

// TEST(parser, parser_test05)
// {
// 	char		*input = "ls -al&&cd&&cat>file;cat>file";
// 	char		*expected_token[] = \
// 		{"ls", "-al", "&&", "cd", "&&", "cat", ">", "file", ";", "cat", ">", "file"};
// 	t_node_kind expected_node[] = \
// 		{ND_COMMAND, ND_DAND, ND_COMMAND, ND_DAND, ND_COMMAND, ND_SEMICOLON, ND_COMMAND};
// 	t_node		*tree = parser(input);

// 	func(tree, expected_token, expected_node);
// }

// TEST(parser, parser_test06)
// {
// 	char		*input = "ls -al ; cat || cat>file||cat>file";
// 	char		*expected_token[] = \
// 		{"ls", "-al", ";", "cat", "||", "cat", ">", "file", "||", "cat", ">", "file"};
// 	t_node_kind expected_node[] = \
// 		{ND_COMMAND, ND_SEMICOLON, ND_COMMAND, ND_DPIPE, ND_COMMAND, ND_DPIPE, ND_COMMAND};
// 	t_node		*tree = parser(input);

// 	func(tree, expected_token, expected_node);
// }

// TEST(parser, parser_test07)
// {
// 	char		*input = "ls -al ; cat ;";
// 	char		*expected_token[] = \
// 		{"ls", "-al", ";", "cat", ";"};
// 	t_node_kind expected_node[] = \
// 		{ND_COMMAND, ND_SEMICOLON, ND_COMMAND, ND_SEMICOLON};
// 	t_node		*tree = parser(input);

// 	func(tree, expected_token, expected_node);
// }

// TEST(parser, parser_test08)
// {
// 	char		*input = "ls -al ; cat ; cd . ; ls -a ;";
// 	char		*expected_token[] = \
// 		{"ls", "-al", ";", "cat", ";", "cd", ".", ";", "ls", "-a", ";"};
// 	t_node_kind expected_node[] = \
// 		{ND_COMMAND, ND_SEMICOLON, ND_COMMAND, ND_SEMICOLON, ND_COMMAND, ND_SEMICOLON, ND_COMMAND, ND_SEMICOLON};
// 	t_node		*tree = parser(input);

// 	func(tree, expected_token, expected_node);
// }

// TEST(parser, parser_test09)
// {
// 	char		*input = "ls -al | | cat ;";
// 	char		*expected_token[] = \
// 		{"ls", "-al", ";", "cat", ";"};
// 	t_node_kind expected_node[] = \
// 		{ND_COMMAND, ND_SEMICOLON, ND_COMMAND, ND_SEMICOLON};
// 	t_node		*tree = parser(input);

// 	func(tree, expected_token, expected_node);
// }