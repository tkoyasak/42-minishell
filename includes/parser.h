#ifndef PARSER_H
# define PARSER_H

# include "libft.h"
# include "struct.h"
# include "lexer.h"

int		parser(t_node **tree, t_list *token_list);
t_node	*parser_error(t_list **itr, char *str, bool *is_valid, int line);
t_node	*node_new(t_node_kind kind, t_node *lhs, t_node *rhs);
bool	consume_node_kind(t_list **itr, char *op);
t_node	*create_process_node(t_list **itr, bool *is_valid);

#endif
