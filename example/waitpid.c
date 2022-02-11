#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(void) {
  int status;

  pid_t pid = fork();
  if (pid == 0) {
	printf("child\n");
    // 子プロセスが実行されている場合
    char *argv[] = {"ls", "-l", NULL};
    char *envp[] = {NULL};
    execve("/bin/ls", argv, envp);
	exit(127);

  } else {

    // 親プロセスが実行
    waitpid(pid, &status, WUNTRACED); // 指定したidの子プロセスを待つ
	printf("21: %d\n", status);
    // exitされたかどうかをチェック
    if (WIFEXITED(status)) {
      // exit()に渡されたステータスの一部を表示（正常終了なら0を表示）
      printf("Exit: %d\n", WEXITSTATUS(status));
    }
  }

}
