#ifndef PARSER_H
# define PARSER_H

# include "lexer.h"

typedef enum e_node_kind
{
	ND_SEMICOLON, // ;
	ND_DAND, // &&
	ND_DPIPE, // ||
	ND_PIPE, // |
	ND_COMMAND // 文字列
}	t_node_kind;

// 抽象構文木のノードの型
typedef struct s_node
{
	t_node_kind		kind; // ノードの型
	struct s_node	*lhs; // 左辺
	struct s_node	*rhs; // 右辺
	t_token			*token; // |間の子プロセス
}	t_node;

t_node	*parser(char *argv);

#endif
