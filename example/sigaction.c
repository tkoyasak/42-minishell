#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

void signal_handler(int signum)
{
	printf("%d\n", signum);
	puts("ぼくはしにましぇん！");
	exit(1);
}

int main(void){

	/* シグナルハンドラの登録 */
	struct sigaction sa;
	sigemptyset(&sa.sa_mask);
	printf("%d\n", sa.sa_mask);
	sigaddset(&sa.sa_mask, 3);
	printf("%u\n", sa.sa_mask);
	sa.sa_handler = signal_handler;
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);

	/*Ctl+Cで強制終了されるまで無限ループ */
	while(1) {
	}

	return 0;
}
