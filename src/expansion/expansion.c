#include "expansion.h"

int	word_len(char *str)
{
	int	idx;

	idx = 0;
	while (str[idx] && !strchr(" $\"'", str[idx]))
		idx++;
	return (idx);
}

int	get_env_len(char *env_word)
{
	extern char	**environ;
	int			idx;

	idx = 0;
	while (environ[idx])
	{
		if (!strncmp(environ[idx], env_word, strlen(env_word)))
			return (strlen(environ[idx]) - strlen(env_word) - 1);
		idx++;
	}
	return (0);
}

int	get_env_index(char *env_word)
{
	extern char	**environ;
	int			idx;

	idx = 0;
	while (environ[idx])
	{
		printf("%s\n", environ[idx]);
		if (!strncmp(environ[idx], env_word, strlen(env_word)))
			return (idx);
		idx++;
	}
	return (-1);
}

char	*lexer_and_revert(char *env_value)
{
	t_token	*token;
	t_token	*head;
	char	*reverted_str;
	size_t	len;

	printf("env_value: %s\n", env_value);
	token = lexer(env_value);
	head = token;
	len = -1;
	while (token->kind != TK_EOF)
	{
		len += strlen(token->str) + 1;
		token = token->next;
		printf("57: %s\n", token->str);
	}
	reverted_str = calloc(len + 1, sizeof(char));
	token = head;
	while (token->kind != TK_EOF)
	{
		strlcat(reverted_str, token->str, strlen(reverted_str) + strlen(token->str) + 1);
		token = token->next;
	}
	return (reverted_str);
}

// var=ls   "$var "'-al'     ls -al
size_t get_expanded_len(char *str)
{
	extern char	**environ;
	char		*env_word;
	char		*env_value;
	size_t		len;
	int			idx;
	int			env_idx;
	bool		has_squote;
	bool		has_dquote;

	len = 0;
	idx = 0;
	has_squote = false;
	has_dquote = false;
	while (str[idx])
	{
		printf("89:%c\n", str[idx]);
		if (!has_dquote && str[idx] == '\'')
			has_squote ^= true;
		else if (!has_squote && str[idx] == '"')
			has_dquote ^= true;
		if (!has_squote && str[idx] == '$')
		{
			idx++; // 次のスペースまでを文字列として見て、環境変数の展開
			env_word = malloc(word_len(&str[idx]) + 1);
			strlcpy(env_word, &str[idx], word_len(&str[idx]) + 1);// $HOGEの時のHOGE
			env_idx = get_env_index(env_word);
			idx += strlen(env_word);
			printf("99 :%d\n", env_idx);
			printf("env_word: %s\n", env_word);
			printf("environ_word: %s\n", environ[env_idx]);
			if (env_idx != -1)
			{
				env_value = strdup(environ[env_idx] + strlen(env_word) + 1);
				if (!has_dquote)
					len += strlen(lexer_and_revert(env_value));
				else
					len += strlen(environ[env_idx]) - strlen(env_word) - 1;
			}
			free(env_word);
		}
		else
		{
			len++;
			idx++;
		}
	}
	return (len);
}

char	*get_expanded_str(char *expanded_str, char *str)
{
	extern char	**environ;
	char		*env_word;
	char		*env_value;
	int			idx;
	int			env_idx;
	char		*word;
	bool		has_squote;
	bool		has_dquote;

	idx = 0;
	has_squote = false;
	has_dquote = false;
	while (str[idx])
	{
		if (!has_dquote && str[idx] == '\'')
			has_squote ^= true;
		else if (!has_squote && str[idx] == '"')
			has_dquote ^= true;
		if (!has_squote && str[idx] == '$')
		{
			idx++; // 次のスペースまでを文字列として見て、環境変数の展開
			env_word = malloc(word_len(&str[idx]) + 1);
			strlcpy(env_word, &str[idx], word_len(&str[idx]) + 1); // $HOGEの時のHOGE
			env_idx = get_env_index(env_word);
			idx += strlen(env_word);
			if (env_idx != -1)
			{
				env_value = strdup(environ[env_idx] + strlen(env_word) + 1);
				if (!has_dquote)
					env_value = lexer_and_revert(env_value);
				strlcat(expanded_str, env_value, strlen(expanded_str) + strlen(env_value) + 1);
				free(env_value);
			}
			free(env_word); // PATH
		}
		else
		{
			printf("162: %c\n", str[idx]);
			strlcat(expanded_str, &str[idx], strlen(expanded_str)+2);
			idx++;
		}
	}
	printf("expanded_str: %s\n", expanded_str);
	return (expanded_str);
}

// abc$PATH
t_token	*get_expanded_token(t_token *token)
{
	char		*expanded_str;
	size_t		expanded_len;

	expanded_len = get_expanded_len(token->str);
	printf("expanded len: %zu\n", expanded_len);
	expanded_str = calloc(expanded_len + 1, sizeof(char));
	// mallocエラー処理
	expanded_str = get_expanded_str(expanded_str, token->str);
	printf("182: expanded str: %s\n", expanded_str);
	token->str = expanded_str;
	// quote削除
	return (token);
}

void	handle_process(t_node *node)
{
	t_token	*token;
	t_token *next;
	t_token *prev;

	token = node->token;
	prev = NULL;
	while (token)
	{
		next = token->next;
		token = get_expanded_token(token); //(token)を展開して先頭の(token)アドレスを返す 1(token) -> 2(token)
		if (prev)
			prev->next = token;
		else
			node->token = token;
		while (token->next != NULL && token->next != next && token->next->kind != TK_EOF)
			token = token->next;
		assert(token->kind != TK_EOF);
		token->next = next;
		prev = token;
		token = token->next;
	}
}
/*
export var = “-a -l”
ls $var NULL
ls -a -l NULL
*/

// dfs
void	expansion_sub(t_node *node)
{
	//今のnodeに対する処理
	if (node->kind == ND_PROCESS)
	{
		handle_process(node); // cmd > cmd = process
	}
	if (node->lhs)
		expansion_sub(node->lhs);
	if (node->rhs)
		expansion_sub(node->rhs);
}

t_node	*expansion(char *argv)
{
	t_token	*tokens;
	t_node	*tree;

	tree = parser(argv);
	expansion_sub(tree);

	return (tree);
}

static void	dfs(t_node *tree)
{
	if (tree->lhs)
		dfs(tree->lhs);
	printf("kind:%d\n", tree->kind);
	while (tree->token)
	{
		printf("%s\n", tree->token->str);
		tree->token = tree->token->next;
	}
	if (tree->rhs)
		dfs(tree->rhs);
}


// int	main(void)
// {
// 	char *s = "ls -al";

// 	// int	len = get_expanded_len(s);
// 	// printf("%s : %d\n", s, len);
// 	t_node	*tree;

// 	tree = expansion("ls -al | cat");
// 	dfs(tree);
// 	return (0);
// }
