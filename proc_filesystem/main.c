#include <stdio.h>
#include <unistd.h>
#include <string.h>



int main(void)
{
    unsigned long int i;
	char *s;

	i = 0;
	s = strdup("Holberton School ");
	while (s)
	{
		printf("[%lu] %s\n", i, s);
		sleep(1);
		i++;
	}
	return (0);
}