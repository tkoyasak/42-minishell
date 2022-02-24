#include <gtest/gtest.h>

extern "C" {
	#include "lexer.h"
	#include "parser.h"
	#include "expansion.h"
	#include "libft.h"
	#include <stdlib.h>
}

int token_idx;
int	node_idx;

void	dfs(t_node *node, char *expected_token[], t_node_kind expected_node[])
{
	if (node->lhs)
		dfs(node->lhs, expected_token, expected_node);
	EXPECT_EQ(node->kind, expected_node[node_idx]);
	node_idx++;
	if (node->kind != ND_PROCESS)
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

char	*strjoin(char *s1, char *s2)
{
	char		*str;

	if (s1 == NULL)
		return (s2);
	if (s2 == NULL)
		return (s1);
	str = (char *)calloc(100, 30);
	strlcpy(str, s1, strlen(s1) + 1);
	strlcat(str, s2, strlen(str) + strlen(s2) + 1);
	return (str);
}

TEST(expansion, expansion_test00)
{
	char		*input = "ls -al | cat aa$zzz$";
	char		*expected_token[] = {"ls", "-al", "|", "cat", strjoin(strjoin("aa", getenv("zzz")),"$")};
	t_node_kind expected_node[] = {ND_PROCESS, ND_PIPE, ND_PROCESS};
	t_node		*tree = expansion(input);

	func(tree, expected_token, expected_node);
}

TEST(expansion, expansion_test01)
{
	char		*input = "ls -al | cat $PATH$PATH";
	char		*expected_token[] = {"ls", "-al", "|", "cat", strjoin(getenv("PATH"), getenv("PATH"))};
	t_node_kind expected_node[] = {ND_PROCESS, ND_PIPE, ND_PROCESS};
	t_node		*tree = expansion(input);

	func(tree, expected_token, expected_node);
}

TEST(expansion, expansion_test02)
{
	char		*input = "ls -al|cat $PATH$";
	char		*expected_token[] = {"ls", "-al", "|", "cat", strjoin(getenv("PATH"), "$")};
	t_node_kind expected_node[] = {ND_PROCESS, ND_PIPE, ND_PROCESS};
	t_node		*tree = expansion(input);

	func(tree, expected_token, expected_node);
}

TEST(expansion, expansion_test03)
{
	char		*input = "ls -al|cat $PATH$ | cat";
	char		*expected_token[] = {"ls", "-al", "|", "cat", strjoin(getenv("PATH"),"$"), "|", "cat"};
	t_node_kind expected_node[] = {ND_PROCESS, ND_PIPE, ND_PROCESS, ND_PIPE, ND_PROCESS};
	t_node		*tree = expansion(input);

	func(tree, expected_token, expected_node);
}

TEST(expansion, expansion_test04)
{
	char		*input = "ls -al|cat $PATH#";
	char		*expected_token[] = {"ls", "-al", "|", "cat", strjoin(getenv("PATH"), "#")};
	t_node_kind expected_node[] = {ND_PROCESS, ND_PIPE, ND_PROCESS};
	t_node		*tree = expansion(input);

	func(tree, expected_token, expected_node);
}

TEST(expansion, expansion_test05)
{
	char		*input = "ls -al|cat $PATH$;";
	char		*expected_token[] = {"ls", "-al", "|", "cat", strjoin(getenv("PATH"),"$"), ";"};
	t_node_kind expected_node[] = {ND_PROCESS, ND_PIPE, ND_PROCESS, ND_SEMICOLON};
	t_node		*tree = expansion(input);

	func(tree, expected_token, expected_node);
}

TEST(expansion, expansion_test06)
{
	char		*input = "ls -al|cat $SHELL$;";
	char		*expected_token[] = {"ls", "-al", "|", "cat", strjoin(getenv("SHELL"),"$"), ";"};
	t_node_kind expected_node[] = {ND_PROCESS, ND_PIPE, ND_PROCESS, ND_SEMICOLON};
	t_node		*tree = expansion(input);

	func(tree, expected_token, expected_node);
}

TEST(expansion, expansion_test07)
{
	char		*input = "$PWD;cat $PWD$;";
	char		*expected_token[] = {getenv("PWD"), ";", "cat", strjoin(getenv("PWD"),"$"), ";"};
	t_node_kind expected_node[] = {ND_PROCESS, ND_SEMICOLON, ND_PROCESS, ND_SEMICOLON};
	t_node		*tree = expansion(input);

	func(tree, expected_token, expected_node);
}

TEST(expansion, expansion_test08)
{
	char		*input = "\"$PWD\" ; \'cat\' \"aa\'aaa\" ;";
	char		*expected_token[] = {getenv("PWD"), ";", "cat", "aa'aaa", ";"};
	t_node_kind expected_node[] = {ND_PROCESS, ND_SEMICOLON, ND_PROCESS, ND_SEMICOLON};
	t_node		*tree = expansion(input);

	func(tree, expected_token, expected_node);
}

TEST(expansion, expansion_test09)
{
	char		*input = "\'$PWD \';cat $PWD$;";
	char		*expected_token[] = {"$PWD ", ";", "cat", strjoin(getenv("PWD"),"$"), ";"};
	t_node_kind expected_node[] = {ND_PROCESS, ND_SEMICOLON, ND_PROCESS, ND_SEMICOLON};
	t_node		*tree = expansion(input);

	func(tree, expected_token, expected_node);
}

TEST(expansion, expansion_test10)
{
	setenv("VAR", "\"hello w\"'orld'", 0);
	char		*input = "$VAR ; \"$VAR\"";
	char		*expected_token[] = {"hello", "world", ";", "hello world"};
	t_node_kind expected_node[] = {ND_PROCESS, ND_SEMICOLON, ND_PROCESS};
	t_node		*tree = expansion(input);

	func(tree, expected_token, expected_node);
	unsetenv("VAR");
}

TEST(expansion, expansion_test11)
{
	setenv("VAR", "$SHELL is zsh", 0);
	char		*input = "$VAR ; \"$VAR\"";
	char		*expected_token[] = {getenv("SHELL"), "is", "zsh", ";", strjoin(getenv("SHELL"), " is zsh")};
	t_node_kind expected_node[] = {ND_PROCESS, ND_SEMICOLON, ND_PROCESS};
	t_node		*tree = expansion(input);

	func(tree, expected_token, expected_node);
	unsetenv("VAR");
}

TEST(expansion, expansion_test12)
{
	setenv("VAR", "hello", 0);
	setenv("VAR2", "\"$VAR $VAR\"", 0);
	char		*input = "$VAR2";
	char		*expected_token[] = {"hello hello"};
	t_node_kind expected_node[] = {ND_PROCESS};
	t_node		*tree = expansion(input);

	func(tree, expected_token, expected_node);
	unsetenv("VAR");
	unsetenv("VAR2");
}

TEST(expansion, expansion_test13)
{
	unsetenv("VAR");
	char		*input = "$VAR | cat";
	char		*expected_token[] = {"", "|", "cat"};
	t_node_kind expected_node[] = {ND_PROCESS, ND_PIPE, ND_PROCESS};
	t_node		*tree = expansion(input);

	func(tree, expected_token, expected_node);
}