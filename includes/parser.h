#ifndef PARSER_H
# define PARSER_H

# include "libft.h"
# include "lexer.h"

typedef enum e_node_kind
{
	ND_SEMICOLON, // ;
	ND_DAND, // &&
	ND_DPIPE, // ||
	ND_PIPE, // |
	ND_PROCESS // 文字列
}	t_node_kind;

// 抽象構文木のノードの型
typedef struct s_node
{
	t_node_kind		kind; // ノードの型
	struct s_node	*lhs; // 左辺
	struct s_node	*rhs; // 右辺
	t_list			*token_list; // ls -> -al の 先頭(ls)
}	t_node;

t_node	*parser(char *argv);

#endif
