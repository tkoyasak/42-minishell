#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
int main()
{
	int pipefd[2];

	if (pipe(pipefd) < 0) {
		perror("pipe");
		exit(-1);
	}
	printf("pipefd: %d %d\n", pipefd[0], pipefd[1]);
	// パイプへの書き込み
	char s[10] = "testtest";
	write(pipefd[1], s, strlen(s)+1); //エラー処理省略

	// パイプからの読み込み
	char buf[128];
	read(pipefd[0], buf, strlen(s)+1); //エラー処理省略

	printf("buf=[%s]\n", buf);

	close(pipefd[1]); //エラー処理省略
	close(pipefd[0]); //エラー処理省略

	return 0;
}
