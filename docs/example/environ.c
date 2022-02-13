#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> /* environ */

int main(int argc, char **argv)
{
    extern char	**environ;
    char		**env;
	
	env = environ;

    while(*env) {
		printf("%s\n", *env);
        env++;
    }

    return 0;
}
