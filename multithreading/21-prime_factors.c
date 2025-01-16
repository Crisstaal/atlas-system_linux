#include "multithreading.h"
#include <stdlib.h>

/**
 * prime_factors - prime factors
 * @s: string
 * Return: a list with the prime factors
 */

list_t *prime_factors(char const *s)
{
	list_t *list = NULL;
	unsigned long n = 0, i = 0, *prime = NULL;

	if (!s)
		return (list);

	list = malloc(sizeof(*list));
	if (!list)
		return (list);
	list = list_init(list), n = strtoul(s, NULL, 10);

	for (i = 2; (i * i) <= n; i += (i == 2) ? 1 : 2)
	{

		while (n % i == 0)
		{
			prime = calloc(1, sizeof(unsigned long));
			*prime = i;
			list_add(list, prime);
			n /= i;
		}
	}

	if (n != 1)
	{
		prime = calloc(1, sizeof(unsigned long));
		*prime = n;
		list_add(list, prime);
	}

	return (list);
}