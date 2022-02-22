#include <gtest/gtest.h>

#define PATH_LOCAL "/Users/jkosaka/.pyenv/shims:/Users/jkosaka/.pyenv/bin:/Users/jkosaka/Library/Python/3.8/bin:/Users/jkosaka/development/flutter/bin:/Users/jkosaka/Desktop/42/push_swap_pre:/opt/homebrew/bin:/opt/homebrew/sbin:/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin:/opt/X11/bin:/Library/Apple/usr/bin:/Users/jkosaka/.cargo/bin:/usr/ucb"
// #define PATH_LOCAL "/Users/koyasako/.nvm/versions/node/v17.4.0/bin:/usr/local/opt/mysql@5.6/bin:/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin:/usr/local/go/bin:/opt/X11/bin:/Library/Apple/usr/bin:/Users/koyasako/.nvm/versions/node/v17.4.0/bin:/usr/local/opt/mysql@5.6/bin:/Users/koyasako/.cargo/bin"

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

	str = (char *)calloc(100, 30);
	strlcpy(str, s1, strlen(s1) + 1);
	strlcat(str, s2, strlen(str) + strlen(s2) + 1);
	return (str);
}

#define STR(x,VAR) (#x VAR)

TEST(expansion, expansion_test00)
{
	char		*input = "ls -al | cat aa$PATH";
	char		*expected_token[] = {"ls", "-al", "|", "cat", STR(aa, PATH_LOCAL)};
	t_node_kind expected_node[] = {ND_PROCESS, ND_PIPE, ND_PROCESS};
	t_node		*tree = expansion(input);

	func(tree, expected_token, expected_node);
}

TEST(expansion, expansion_test01)
{
	char		*input = "ls -al | cat $PATH$PATH";
	char		*expected_token[] = {"ls", "-al", "|", "cat", strjoin(PATH_LOCAL, PATH_LOCAL)};
	t_node_kind expected_node[] = {ND_PROCESS, ND_PIPE, ND_PROCESS};
	t_node		*tree = expansion(input);

	func(tree, expected_token, expected_node);
}

TEST(expansion, expansion_test02)
{
	char		*input = "ls -al|cat $PATH$";
	char		*expected_token[] = {"ls", "-al", "|", "cat", strjoin(PATH_LOCAL,"$")};
	t_node_kind expected_node[] = {ND_PROCESS, ND_PIPE, ND_PROCESS};
	t_node		*tree = expansion(input);

	func(tree, expected_token, expected_node);
}

TEST(expansion, expansion_test03)
{
	char		*input = "ls -al|cat $PATH$ | cat";
	char		*expected_token[] = {"ls", "-al", "|", "cat", strjoin(PATH_LOCAL,"$"), "|", "cat"};
	t_node_kind expected_node[] = {ND_PROCESS, ND_PIPE, ND_PROCESS, ND_PIPE, ND_PROCESS};
	t_node		*tree = expansion(input);

	func(tree, expected_token, expected_node);
}

TEST(expansion, expansion_test04)
{
	char		*input = "ls -al|cat $PATH#";
	char		*expected_token[] = {"ls", "-al", "|", "cat", strjoin(PATH_LOCAL, "#")};
	t_node_kind expected_node[] = {ND_PROCESS, ND_PIPE, ND_PROCESS};
	t_node		*tree = expansion(input);

	func(tree, expected_token, expected_node);
}

TEST(expansion, expansion_test05)
{
	char		*input = "ls -al|cat $PATH$;";
	char		*expected_token[] = {"ls", "-al", "|", "cat", strjoin(PATH_LOCAL,"$"), ";"};
	t_node_kind expected_node[] = {ND_PROCESS, ND_PIPE, ND_PROCESS, ND_SEMICOLON};
	t_node		*tree = expansion(input);

	func(tree, expected_token, expected_node);
}