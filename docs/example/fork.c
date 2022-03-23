#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>  // fork
#include <unistd.h>     // fork
#include <err.h>
#include <errno.h>

// int	main(void)
// {
// 	printf("===\n");
// }

int	main(int argc, char *argv[])
{
	pid_t   pid; // proc id

	pid = fork (); // 親プロセスと子プロセスに別れる
	if (-1 == pid)
	{
		err (EXIT_FAILURE, "can not fork");
	}
	else if (0 == pid)
	{
		puts ("child");
	}
	else
	{
		// waitpid(pid);
		puts ("parents");
		printf("child pid %d\n", pid);
	}
	exit (EXIT_SUCCESS);
}
