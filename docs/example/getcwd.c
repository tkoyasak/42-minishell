#include <stdio.h>
#include <unistd.h>
#define PATH_SIZE 50

int main()
{
    char path_name[PATH_SIZE];
    getcwd(path_name, PATH_SIZE);
	printf("%p\n", path_name);
    printf("current dir : |%s|\n", path_name);
    return 0;
}
