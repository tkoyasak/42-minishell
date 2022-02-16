#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

typedef enum e_token_kind
{
	TK_RESERVED,
	TK_STRING,
	TK_EOF
}	t_token_kind;

// <, >, <<, >>, ;, &&, ||, |

typedef struct s_token
{
	t_token_kind	kind;
	struct s_token	*next;
	char			*str;
}	t_token;

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
	int qidx;

	idx = 0;
	while (p[idx] && isspace(p[idx]) == false)
	{
		// if (p[idx] == '\'')
		// {
		// 	idx++;
		// 	while (p[idx] && p[idx] != '\'')
		// 		idx++;
		// 	if (p[idx] != '\'')
		// 		return (0);
		// }
		// if (p[idx] == '"')
		// {
		// 	idx++;
		// 	while (p[idx] && p[idx] != '"')
		// 		idx++;
		// 	if (p[idx] != '"')
		// 		return (0);
		// }
		if (p[idx] == '\'' || p[idx] == '"')
		{
			qidx = 0;
			qidx++;
			while (p[idx + qidx] && p[idx + qidx] != p[idx])
				qidx++;
			if (p[idx + qidx] != p[idx])
				return (0);
			idx += qidx;
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

int	main(int argc, char **argv)
{
	t_token	*token;

	argv[1] = "echo \"aa \"'b'b";
	token = tokenize(argv[1]);

	while (token)
	{
		printf("%s\n", token->str);
		token = token->next;
	}
}
