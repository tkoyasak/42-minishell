#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

typedef enum {
  ND_ADD, // +
  ND_SUB, // -
  ND_MUL, // *
  ND_DIV, // /
  ND_NUM // 整数
} NodeKind;

typedef struct Node Node;

// 抽象構文木のノードの型
struct Node {
  NodeKind kind; // ノードの型
  Node *lhs;     // 左辺
  Node *rhs;     // 右辺
  int val;       // kindがND_NUMの場合のみ使う
};

typedef enum {
  TK_RESERVED, // 記号
  TK_NUM,      // 整数トークン
  TK_EOF,      // 入力の終わりを表すトークン
} TokenKind;

typedef struct Token Token;

// トークン型
struct Token {
  TokenKind kind; // トークンの型
  Token *next;    // 次の入力トークン
  int val;        // kindがTK_NUMの場合、その数値
  char *str;      // トークン文字列
};

Token *token;

Node *new_node(NodeKind kind, Node *lhs, Node *rhs) {
  Node *node = calloc(1, sizeof(Node));
  node->kind = kind;
  node->lhs = lhs;
  node->rhs = rhs;
  return node;
}

Node *new_node_num(int val) {
  Node *node = calloc(1, sizeof(Node));
  node->kind = ND_NUM;
  node->val = val;
  return node;
}

Node *expr();
bool consume(char op)
{
  // token->str が kindに一致したらtrueを返してtokenを進める
  if (token->str[0] == op) {
    token = token->next;
    return true;
  }
  return false;
}

void expect(char op) {
  if (token->str[0] == op) {
    token = token->next;
  } else {
    printf("'%c'ではありません", op);
    exit(1);
  }
}

int expect_number() {
  if (token->kind != TK_NUM) {
    printf("数ではありません");
    exit(1);
  }
  int val = token->val;
  token = token->next;
  return val;
}

// create_subshell_treeに対応
Node *primary() {
  // 次のトークンが"("なら、"(" expr ")"のはず
  if (consume('(')) {
    Node *node = expr();
    expect(')');
    return node;
  }
  // そうでなければ数値のはず
  return new_node_num(expect_number());
}

// create_sub_astreeに対応
Node *mul() {
  Node *node = primary();

  for (;;) {
    if (consume('*'))
      node = new_node(ND_MUL, node, primary());
    else if (consume('/'))
      node = new_node(ND_DIV, node, primary());
    else
      return node;
  }
}

// create_astreeに対応
Node *expr() {
  Node *node = mul();

  for (;;) {
    if (consume('+'))
      node = new_node(ND_ADD, node, mul());
    else if (consume('-'))
      node = new_node(ND_SUB, node, mul());
    else
      return node;
  }
}

// 新しいトークンを作成してcurに繋げる
Token *new_token(TokenKind kind, Token *cur, char *str) {
  Token *tok = calloc(1, sizeof(Token));
  tok->kind = kind;
  tok->str = str;
  cur->next = tok;
  return tok;
}

// 入力文字列pをトークナイズしてそれを返す
Token *tokenize(char *p) {
  Token head;
  head.next = NULL;
  Token *cur = &head;

  while (*p) {
    if (isspace(*p)) {
      p++;
      continue;
    }
    if (strchr("+-*/()", *p)) {
      cur = new_token(TK_RESERVED, cur, p++);
      continue;
    }
    if (isdigit(*p)) {
      cur = new_token(TK_NUM, cur, p);
      cur->val = strtol(p, &p, 10);
      continue;
    }
    printf("トークナイズできません");
    exit(1);
  }
  new_token(TK_EOF, cur, p);
  return head.next;
}

int cal(Node *node) {
  if (node->kind == ND_NUM) {
    return node->val;
  }
  int left = cal(node->lhs);
  int right = cal(node->rhs);
  if (node->kind == ND_ADD) {
    return left + right;
  } else if (node->kind == ND_SUB) {
    return left - right;
  } else if (node->kind == ND_MUL) {
    return left * right;
  } else if (node->kind == ND_DIV) {
    return left / right;
  }
  return -1;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    return 1;
  }

  token = tokenize(argv[1]);

  Node *tree = expr();

  printf("%d\n", cal(tree));

  return 0;
}
