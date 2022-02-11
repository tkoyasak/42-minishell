#include <unistd.h>
#include <stdio.h>

#define FILE_PATH "./test.dat"

int main (void)
{
  /* 読み込み許可と実効許可があるかをチェック */
  if (access (FILE_PATH, (R_OK | X_OK)))
    {
      perror (FILE_PATH);
      return -1;
    }

  printf ("Confirm result (R_OK | X_OK): PASS!\n");
  return 0;
}
