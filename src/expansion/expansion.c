#include "expansion.h"

int	word_len(char *str)
{
	int	idx;

	idx = 0;
	while (str[idx] && str[idx] != ' ')
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
		if (!strncmp(environ[idx], env_word, strlen(env_word)))
			return (idx);
		idx++;
	}
	return (-1);
}

// abc$PATH
t_token	*func2(t_token *token)
{
	char		*str;
	char		*env_word;
	char		*expanded_words;
	extern char	**environ;
	size_t		len;
	int			idx;
	int			env_idx;

	str = token->str;
	len = 0;
	idx = 0;
	while (str[idx])
	{
		if (str[idx] == '$')
		{
			idx++; // 次のスペースまでを文字列として見て、環境変数の展開
			env_word = malloc(word_len(&str[idx]) + 1);
			strlcpy(env_word, &str[idx], word_len(&str[idx]) + 1);// $HOGEの時のHOGE
			env_idx = get_env_index(env_word);
			idx += strlen(env_word);
			if (env_idx != -1)
			{
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
	expanded_words = calloc(len, sizeof(char));
	// mallocエラー処理
	idx = 0;
	while (str[idx])
	{
		if (str[idx] == '$')
		{
			// 次のスペースまでを文字列として見て、環境変数の展開
			idx++; // 次のスペースまでを文字列として見て、環境変数の展開
			env_word = malloc(word_len(&str[idx]) + 1);
			strlcpy(env_word, &str[idx], word_len(&str[idx]) + 1); // $HOGEの時のHOGE
			env_idx = get_env_index(env_word);
			idx += strlen(env_word);
			free(env_word);
			if (env_idx != -1)
			{
				env_word = strdup(environ[env_idx] + strlen(env_word) + 1);
				strlcat(expanded_words, env_word, strlen(expanded_words) + strlen(env_word) + 1);
				free(env_word);
			}
		}
		else
		{
			strlcat(expanded_words, &str[idx], strlen(expanded_words)+2);
			idx++;
		}
	}
	return (lexer(expanded_words));
}

void	func(t_token **token)
{
	t_token *next;
	t_token *prev;

	prev = NULL;
	while (*token)
	{
		next = (*token)->next;
		(*token) = func2((*token)); //(*token)を展開して先頭の(*token)アドレスを返す 1(*token) -> 2(*token)
		if (prev)
			prev->next = (*token);
		while ((*token)->next != NULL && (*token)->next != next)
			(*token) = (*token)->next;
		(*token)->next = next;
		prev = (*token);
		(*token) = (*token)->next;
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
	if (node->kind == ND_COMMAND)
	{
		func(&(node->token));
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

	// (void)argc;
	tree = parser(argv);
	expansion_sub(tree);

	return (tree);
}

// int	main(void)
// {
// 	t_node	*tree;

// 	tree = expansion("ls -a | cat $PATH");
// 	return (0);
// }
