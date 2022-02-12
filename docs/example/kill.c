#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

int pp;
int main()
{
  int         p_id;

  if ((p_id = fork()) == 0) {
    /* 子プロセス */
    while (1) {
      printf("子プロセス実行中\n");
      sleep(1);
	//   kill(pp, SIGINT);
    }
  }
  else {
	pp = getpid();
    /* 親プロセス */
    sleep(3);

    /* 子プロセスに終了シグナルを送信 */
    kill(p_id, SIGINT);

    wait(NULL);
    printf("親プロセス終了\n");
  }

  return 0;
}