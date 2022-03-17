#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
  struct stat      stat_buf;
  int	fd;

  fd = open(argv[1], O_RDONLY);

 if (fstat(0, &stat_buf) == 0) {
    /* ファイル情報を表示 */
    printf("アクセス保護 : %o\n",stat_buf.st_mode);
    printf("容量（バイト単位） : %lld\n",stat_buf.st_size);
 }

  return EXIT_SUCCESS;
}
