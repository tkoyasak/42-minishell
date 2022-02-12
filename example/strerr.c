#include <stdio.h>
#include <errno.h>
#include <string.h>

int main(void) {
	/* グローバル変数のerrnoをクリア */
	errno = 0;
	fprintf(stdout, "1.errno[%d]\n", errno);

	/* わざと存在しないファイルをOpenしてエラーを発生させる */
	FILE *fp = fopen("dummy.txt", "r");
	if (fp == NULL) {
		/* errnoの値を確認 */
		fprintf(stdout, "2.errno[%d]\n", errno);
		fprintf(stdout, "3.strerror(%d):%s\n", errno, strerror(errno));
	} else {
		fclose(fp);
	}

	/*
	 #define EPERM 1
	 #define ENOENT 2
	 #define ENOFILE ENOENT
	 #define ESRCH 3
	 #define EINTR 4
	 #define EIO 5
	 :
	 */
	/* 試しに 1～5のエラー文字列を取得して出力してみる */
	fprintf(stdout, "4.strerror(%d):%s\n", EPERM, strerror(EPERM));
	fprintf(stdout, "5.strerror(%d):%s\n", ENOENT, strerror(ENOENT));
	fprintf(stdout, "6.strerror(%d):%s\n", ESRCH, strerror(ESRCH));
	fprintf(stdout, "7.strerror(%d):%s\n", EINTR, strerror(EINTR));
	fprintf(stdout, "8.strerror(%d):%s\n", EIO, strerror(EIO));
}
