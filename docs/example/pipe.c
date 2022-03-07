#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main()
{
	pid_t   pid;
	int 	pipefd[2];
	char s[10] = "testtest";

	if (pipe(pipefd) < 0) {
		perror("pipe");
		exit(-1);
	}
	printf("pipefd: %d %d\n", pipefd[0], pipefd[1]);
	pid = fork();
	// パイプへの書き込み
	if (pid == 0)
	{
		char *argv[] = {"ls", "-a", NULL};
		char *envp[] = {NULL};
		puts ("child");
		dup2(pipefd[1], 1); // 1 -> 4
		close(pipefd[1]); //エラー処理省略
		close(pipefd[0]); //エラー処理省略
		execve("/bin/ls", argv, envp); //fd = 1に書き込む
		// write(pipefd[1], s, strlen(s)+1); //エラー処理省略
		printf("=========\n");
		exit(0);
	}
	else
	{
		int	wstatus;
		waitpid(pid, &wstatus, WUNTRACED);
		puts ("parents");
		printf("child pid %d\n", pid);
		char buf[128];
		read(pipefd[0], buf, 300); //エラー処理省略
		printf("buf=[%s]\n", buf);
	}

	// パイプからの読み込み
	close(pipefd[1]); //エラー処理省略
	close(pipefd[0]); //エラー処理省略

	return 0;
}
