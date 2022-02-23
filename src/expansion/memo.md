1. “” で囲まれていない環境変数はlexerを通す
2. “” で囲まれている環境変数はそのまま
3. ‘’ で囲まれている$は展開しない
4. 最後に””と’’を削除

token->str
ls -al | cat aa$PATH$

node 3つ
token 5つ

export VAR=bb    $PATH

aa$VAR$
aa 2
$VAR -> bb $PATH 
$

- 文字列
- 文字列の長さ
- ダブルクォートの中か外か -> スプリットするかしないか

# t_token	*get_expanded_token(t_token *token)でやること
