#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "list.h"
#include "multithreading.h"

/***
 * is_prime - Checks if a number is prime
 *
 * @n: The number to check
 *
 * Return: 1 if prime, 0 if not
 ***/
static int is_prime(unsigned long n)
{
    unsigned long i;

    if (n < 2)
        return (0);
    for (i = 2; i * i <= n; i++)
    {
        if (n % i == 0)
            return (0);
    }
    return (1);
}

/***
 * prime_factors - Factorizes a number into its prime factors
 *
 * @s: The string representation of the number to factorize
 *
 * Return: A list of prime factors
 ***/
list_t *prime_factors(char const *s)
{
    unsigned long n;
    list_t *factors = list_create();
    node_t *node;
    char *endptr;

    n = strtoul(s, &endptr, 10);
    if (*endptr != '\0' || n == 0)
    {
        return (NULL);  // invalid input or zero number
    }

    /*** Find factors starting from 2 ***/
    for (unsigned long i = 2; i <= n; i++)
    {
        while (n % i == 0 && is_prime(i))
        {
            node = list_node_create(&i);
            list_add(factors, node);
            n /= i;
        }
        if (n == 1)
            break;
    }

    return (factors);
}