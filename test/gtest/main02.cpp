#include <gtest/gtest.h>

extern "C" {
	#include "lexer.h"
	#include "parser.h"
	#include "expansion.h"
	#include "shell_var.h"
	#include "libft.h"
	#include <stdlib.h>
}

int token_idx;
int	node_idx;

static t_shell_var	*initialize_shell_var(void)
{
	t_shell_var	*shell_var;

	shell_var = (t_shell_var *)calloc(1, sizeof(t_shell_var));
	shell_var->env_list = init_envlist();
	return (shell_var);
}

t_shell_var	*g_shell_var = initialize_shell_var();

void	dfs(t_node *node, char *expected_token[], t_node_kind expected_node[])
{
	if (node->lhs)
		dfs(node->lhs, expected_token, expected_node);
	EXPECT_EQ(node->kind, expected_node[node_idx]);
	node_idx++;
	if (node->kind != ND_PROCESS)
		token_idx++;
	t_list *itr = node->token_list;
	while (node->kind == ND_PROCESS && itr)
	{
		t_token	*token = ((t_token *)(itr->content));
		EXPECT_STREQ(token->str, expected_token[token_idx]);
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
	char		*expected_token[] = {"ls", "-al", "|", "cat", strjoin(strjoin("aa", get_env_value("zzz", g_shell_var)),"$")};
	t_node_kind expected_node[] = {ND_PROCESS, ND_PIPE, ND_PROCESS};
	t_node		*tree = expansion(input, g_shell_var);

	func(tree, expected_token, expected_node);
}

TEST(expansion, expansion_test01)
{
	char		*input = "ls -al | cat $PATH$PATH";
	char		*expected_token[] = {"ls", "-al", "|", "cat", strjoin(get_env_value("PATH", g_shell_var), get_env_value("PATH", g_shell_var))};
	t_node_kind expected_node[] = {ND_PROCESS, ND_PIPE, ND_PROCESS};
	t_node		*tree = expansion(input, g_shell_var);

	func(tree, expected_token, expected_node);
}

TEST(expansion, expansion_test02)
{
	char		*input = "ls -al|cat $PATH$";
	char		*expected_token[] = {"ls", "-al", "|", "cat", strjoin(get_env_value("PATH", g_shell_var), "$")};
	t_node_kind expected_node[] = {ND_PROCESS, ND_PIPE, ND_PROCESS};
	t_node		*tree = expansion(input, g_shell_var);

	func(tree, expected_token, expected_node);
}

TEST(expansion, expansion_test03)
{
	char		*input = "ls -al|cat $PATH$ | cat";
	char		*expected_token[] = {"ls", "-al", "|", "cat", strjoin(get_env_value("PATH", g_shell_var),"$"), "|", "cat"};
	t_node_kind expected_node[] = {ND_PROCESS, ND_PIPE, ND_PROCESS, ND_PIPE, ND_PROCESS};
	t_node		*tree = expansion(input, g_shell_var);

	func(tree, expected_token, expected_node);
}

TEST(expansion, expansion_test04)
{
	char		*input = "ls -al|cat $PATH#";
	char		*expected_token[] = {"ls", "-al", "|", "cat", strjoin(get_env_value("PATH", g_shell_var), "#")};
	t_node_kind expected_node[] = {ND_PROCESS, ND_PIPE, ND_PROCESS};
	t_node		*tree = expansion(input, g_shell_var);

	func(tree, expected_token, expected_node);
}

TEST(expansion, expansion_test05)
{
	char		*input = "ls -al|cat $PATH$;";
	char		*expected_token[] = {"ls", "-al", "|", "cat", strjoin(get_env_value("PATH", g_shell_var),"$"), ";"};
	t_node_kind expected_node[] = {ND_PROCESS, ND_PIPE, ND_PROCESS, ND_SEMICOLON};
	t_node		*tree = expansion(input, g_shell_var);

	func(tree, expected_token, expected_node);
}

TEST(expansion, expansion_test06)
{
	char		*input = "ls -al|cat $SHELL$;";
	char		*expected_token[] = {"ls", "-al", "|", "cat", strjoin(get_env_value("SHELL", g_shell_var),"$"), ";"};
	t_node_kind expected_node[] = {ND_PROCESS, ND_PIPE, ND_PROCESS, ND_SEMICOLON};
	t_node		*tree = expansion(input, g_shell_var);

	func(tree, expected_token, expected_node);
}

TEST(expansion, expansion_test07)
{
	char		*input = "$PWD;cat $PWD$;";
	char		*expected_token[] = {get_env_value("PWD", g_shell_var), ";", "cat", strjoin(get_env_value("PWD", g_shell_var),"$"), ";"};
	t_node_kind expected_node[] = {ND_PROCESS, ND_SEMICOLON, ND_PROCESS, ND_SEMICOLON};
	t_node		*tree = expansion(input, g_shell_var);

	func(tree, expected_token, expected_node);
}

TEST(expansion, expansion_test08)
{
	char		*input = "\"$PWD\" ; \'cat\' \"aa\'aaa\" ;";
	char		*expected_token[] = {get_env_value("PWD", g_shell_var), ";", "cat", "aa'aaa", ";"};
	t_node_kind expected_node[] = {ND_PROCESS, ND_SEMICOLON, ND_PROCESS, ND_SEMICOLON};
	t_node		*tree = expansion(input, g_shell_var);

	func(tree, expected_token, expected_node);
}

TEST(expansion, expansion_test09)
{
	char		*input = "\'$PWD \';cat $PWD$;";
	char		*expected_token[] = {"$PWD ", ";", "cat", strjoin(get_env_value("PWD", g_shell_var),"$"), ";"};
	t_node_kind expected_node[] = {ND_PROCESS, ND_SEMICOLON, ND_PROCESS, ND_SEMICOLON};
	t_node		*tree = expansion(input, g_shell_var);

	func(tree, expected_token, expected_node);
}

// TEST(expansion, expansion_test10)
// {
// 	setenv("VAR", "hello world", 0);
// 	char		*input = "$VAR ; \"$VAR\"";
// 	char		*expected_token[] = {"hello", "world", ";", "hello world"};
// 	t_node_kind expected_node[] = {ND_PROCESS, ND_SEMICOLON, ND_PROCESS};
// 	t_node		*tree = expansion(input, g_shell_var);

// 	func(tree, expected_token, expected_node);
// 	unsetenv("VAR");
// }

// TEST(expansion, expansion_test11)
// {
// 	setenv("VAR", "$SHELL is zsh", 0);
// 	char		*input = "$VAR ; \"$VAR\"";
// 	char		*expected_token[] = {get_env_value("SHELL", g_shell_var), "is", "zsh", ";", strjoin(get_env_value("SHELL", g_shell_var), " is zsh")};
// 	t_node_kind expected_node[] = {ND_PROCESS, ND_SEMICOLON, ND_PROCESS};
// 	t_node		*tree = expansion(input, g_shell_var);

// 	func(tree, expected_token, expected_node);
// 	unsetenv("VAR");
// }

// TEST(expansion, expansion_test12)
// {
// 	setenv("VAR", "hello", 0);
// 	setenv("VAR2", "\"$VAR $VAR\"", 0);
// 	char		*input = "$VAR2";
// 	char		*expected_token[] = {"hello hello"};
// 	t_node_kind expected_node[] = {ND_PROCESS};
// 	t_node		*tree = expansion(input, g_shell_var);

// 	func(tree, expected_token, expected_node);
// 	unsetenv("VAR");
// 	unsetenv("VAR2");
// }

/* TEST for get_expansion_list */

void	func_get_expansion_list(t_list *expansion_list, char *expected_expansion_list[], t_expansion_kind expected_exp_kind[])
{
	int		idx;

	idx = 0;
	while (expansion_list)
	{
		EXPECT_STREQ(((t_expansion *)(expansion_list->content))->str, expected_expansion_list[idx]);
		EXPECT_EQ(((t_expansion *)(expansion_list->content))->kind, expected_exp_kind[idx]);
		expansion_list = expansion_list->next;
		idx++;
	}
}

// TEST(get_expansion_list, expansion_list_test00)
// {
// 	setenv("VAR", "hello", 0);
// 	char		*input = "$VAR | cat";
// 	char		*expected_expansion_list[] = {"hello", " ", "|", " ", "cat"};
// 	t_expansion_kind	expected_exp_kind[] = {STRING, NAKED_SPACE, STRING, NAKED_SPACE, STRING};
// 	t_list *res = get_expansion_list(input, false);

// 	func_get_expansion_list(res, expected_expansion_list, expected_exp_kind);
// 	unsetenv("VAR");
// }

// TEST(get_expansion_list, expansion_list_test01)
// {
// 	setenv("VAR", "hello world", 0);
// 	char		*input = "$VAR$PATH '| cat'";
// 	char		*expected_expansion_list[] = {"hello", " ", "world", get_env_value("PATH"), " ", "\'", "| cat", "\'"};
// 	t_expansion_kind	expected_exp_kind[] = {STRING, NAKED_SPACE, STRING, STRING, NAKED_SPACE, SQUOTE, STRING, SQUOTE};
// 	t_list *res = get_expansion_list(input, false);

// 	func_get_expansion_list(res, expected_expansion_list, expected_exp_kind);
// 	unsetenv("VAR");
// }

// TEST(get_expansion_list, expansion_list_test02)
// {
// 	setenv("VAR", "'hell''o world'", 0);
// 	char		*input = "$VAR$ | $";
// 	char		*expected_expansion_list[] = {"\'", "hell", "\'", "\'", "o world", "\'", "$", " ", "|", " ", "$"};
// 	t_expansion_kind	expected_exp_kind[] = {SQUOTE, STRING, SQUOTE, SQUOTE, STRING, SQUOTE, STRING, NAKED_SPACE, STRING, NAKED_SPACE, STRING};
// 	t_list *res = get_expansion_list(input, false);

// 	func_get_expansion_list(res, expected_expansion_list, expected_exp_kind);
// 	unsetenv("VAR");
// }

// TEST(get_expansion_list, expansion_list_test03)
// {
// 	setenv("VAR1", "hello world", 0);
// 	setenv("VAR2", "$VAR1", 0);
// 	char		*input = "a$VAR2";
// 	char		*expected_expansion_list[] = {"a", "hello", " ", "world"};
// 	t_expansion_kind	expected_exp_kind[] = {STRING, STRING, NAKED_SPACE, STRING};
// 	t_list *res = get_expansion_list(input, false);

// 	func_get_expansion_list(res, expected_expansion_list, expected_exp_kind);
// 	unsetenv("VAR1");
// 	unsetenv("VAR2");
// }

// TEST(get_expansion_list, expansion_list_test04)
// {
// 	setenv("VAR1", "hello  world", 0);
// 	setenv("VAR2", "$VAR1", 0);
// 	char		*input = "a\"$VAR2\"";
// 	char		*expected_expansion_list[] = {"a", "\"", "hello  world", "\""};
// 	t_expansion_kind	expected_exp_kind[] = {STRING, DQUOTE, STRING, DQUOTE};
// 	t_list *res = get_expansion_list(input, false);

// 	func_get_expansion_list(res, expected_expansion_list, expected_exp_kind);
// 	unsetenv("VAR1");
// 	unsetenv("VAR2");
// }

/* TEST for remove_quotes */

// TEST(remove_quotes, remove_quotes_test00)
// {
// 	setenv("VAR", "hello  world", 0);
// 	char		*input = "a\"$VAR\"";
// 	char		*expected_expansion_list[] = {"a", "hello  world"};
// 	t_expansion_kind	expected_exp_kind[] = {STRING, STRING};
// 	t_list *res = get_expansion_list(input, false);
// 	res = remove_quotes(res);

// 	func_get_expansion_list(res, expected_expansion_list, expected_exp_kind);
// 	unsetenv("VAR");
// }

// TEST(remove_quotes, remove_quotes_test01)
// {
// 	setenv("VAR", "hello  world", 0);
// 	char		*input = "a$VAR";
// 	char		*expected_expansion_list[] = {"a", "hello", "  ",  "world"};
// 	t_expansion_kind	expected_exp_kind[] = {STRING, STRING, NAKED_SPACE, STRING};
// 	t_list *res = get_expansion_list(input, false);
// 	res = remove_quotes(res);

// 	func_get_expansion_list(res, expected_expansion_list, expected_exp_kind);
// 	unsetenv("VAR");
// }

// TEST(remove_quotes, remove_quotes_test02)
// {
// 	setenv("VAR", "hello  w'orl'd", 0);
// 	char		*input = "a\"$VAR\"";
// 	char		*expected_expansion_list[] = {"a", "hello  w'orl'd"};
// 	t_expansion_kind	expected_exp_kind[] = {STRING, STRING};
// 	t_list *res = get_expansion_list(input, false);
// 	res = remove_quotes(res);

// 	func_get_expansion_list(res, expected_expansion_list, expected_exp_kind);
// 	unsetenv("VAR");
// }

// TEST(remove_quotes, remove_quotes_test03)
// {
// 	setenv("VAR", "hello  w'orl'd", 0);
// 	char		*input = "a$VAR";
// 	char		*expected_expansion_list[] = {"a", "hello", "  ", "w", "orl", "d"};
// 	t_expansion_kind	expected_exp_kind[] = {STRING, STRING, NAKED_SPACE, STRING, STRING, STRING};
// 	t_list *res = get_expansion_list(input, false);
// 	res = remove_quotes(res);

// 	func_get_expansion_list(res, expected_expansion_list, expected_exp_kind);
// 	unsetenv("VAR");
// }
