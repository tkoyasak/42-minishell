#include "lexer.h"

t_token *tk_token_new(t_token_kind kind, char *str)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	token->kind = kind;
	token->str = str;
	token->next = NULL;
	return (token);
}

t_token	*tk_token_last(t_token *token)
{
	if (!token)
		return (NULL);
	while (token->next)
		token = token->next;
	return (token);
}

void	tk_token_add_back(t_token **head, t_token *newsrc)
{
	if (!head)
		return ;
	if (!*head)
		*head = newsrc;
	else
		(tk_token_last(*head))->next = newsrc;
}
