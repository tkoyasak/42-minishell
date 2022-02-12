#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
	system("ls");
	printf("%d\n", unlink("slink"));
	system("ls");
	printf("%d\n", unlink("dammy"));
}
