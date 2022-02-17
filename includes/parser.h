#ifndef PARSER_H
# define PARSER_H

# include "lexer.h"

typedef enum e_node_kind 
{
	ND_DQUOTE, // "
	ND_SQUOTE, // '
	ND_PIPE, // |
	ND_DPIPE, // ||
	ND_LESS, // <
	ND_DLESS, // <<
	ND_GTR, // >
	ND_DGTR, // >>
	ND_AND, // &
	ND_DAND, // &&
	ND_SEMICOLON, // ;
	ND_STR // 文字列
}	t_node_kind;

// 抽象構文木のノードの型
typedef struct s_node
{
	t_node_kind		kind; // ノードの型
	struct s_node	*lhs; // 左辺
	struct s_node	*rhs; // 右辺
	int val;			 // kindがND_NUMの場合のみ使う
}	t_node;

#endif
