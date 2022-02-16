#include "lexer.h"

// <<< などを弾く
bool	is_valid_str(char *p)
{
	int	idx;

	idx = 0;
	while (strchr("<>|&;", p[idx]))
		idx++;
	if (idx == 0)
		return (true);
	if (idx > 2)
		return (false);
	if (idx == 2 && p[0] != p[1])
		return (false);
	if (p[idx] && isspace(p[idx]) == false)
		return (false);
	return (true);
}

int	reserved_len(char *p)
{
	int	idx;

	idx = 0;
	while (strchr("<>|&;", p[idx]))
		idx++;
	return (idx);
}

int	string_len(char *p)
{
	int	idx;
	int quote_idx;

	idx = 0;
	while (p[idx] && isspace(p[idx]) == false)
	{
		if (p[idx] == '\'' || p[idx] == '"')
		{
			quote_idx = 1;
			while (p[idx + quote_idx] && p[idx + quote_idx] != p[idx])
				quote_idx++;
			if (p[idx + quote_idx] != p[idx])
				return (0);
			idx += quote_idx;
		}
		idx++;
	}
	return (idx);
}

t_token	*new_token(t_token_kind kind, t_token *cur, char **p, int len)
{
	t_token	*tok;

	tok = calloc(1, sizeof(t_token));
	tok->kind = kind;
	cur->next = tok;
	tok->str = malloc((len + 1) * sizeof(char));
	strlcpy(tok->str, *p, len + 1);
	*p += len;
	return (tok);
}

t_token	*tokenize(char *p)
{
	t_token	head;
	t_token	*cur;
	int		len;

	head.next = NULL;
	cur = &head;
	while (*p)
	{
		if (isspace(*p))
		{
			p++;
			continue;
		}
		if (is_valid_str(p) == false)
		{
			exit(1);
		}
		len = reserved_len(p);
		if (len > 0)
		{
			cur = new_token(TK_RESERVED, cur, &p, len);
			continue ;
		}
		len = string_len(p);
		if (len > 0)
		{
			cur = new_token(TK_STRING, cur, &p, len);
			continue ;
		}
		// free必要
		// error_handler(free_all(&head, ___));
		printf("quotes error\n");
		exit(1);
	}
	new_token(TK_EOF, cur, &p, 0);
	return (head.next);
}

// int	main(int argc, char **argv)
// {
// 	t_token	*token;

// 	// argv[1] = "echo \"aa \"'b'b";
// 	argv[1] = "echo \"hello w\"'orld'";
// 	token = tokenize(argv[1]);

// 	while (token)
// 	{
// 		printf("type:%d %s\n", token->kind, token->str);
// 		token = token->next;
// 	}
// }

t_token	*lexer(int argc, char *argv)
{
	t_token	*token;

	(void)argc;
	token = tokenize(argv);

	// while (token)
	// {
	// 	printf("%s\n", token->str);
	// 	token = token->next;
	// }
	return (token);
}
