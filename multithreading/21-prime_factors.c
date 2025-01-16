#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "list.h"

/**
 * is_prime - Checks if a number is prime
 *
 * @n: The number to check
 *
 * Return: 1 if prime, 0 if not
 */
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

/**
 * prime_factors - Factorizes a number into its prime factors
 *
 * @s: The string representation of the number to factorize
 *
 * Return: A list_t structure containing the prime factors
 */
list_t *prime_factors(char const *s)
{
    list_t *factors;
    unsigned long num;
    unsigned long divisor;

    /* Convert the string to an unsigned long */
    num = strtoul(s, NULL, 10);
    
    /* Check if the number is valid */
    if (num == 0)
        return (NULL);

    factors = list_init(malloc(sizeof(list_t)));

    /* Handle factorization */
    divisor = 2;
    while (num > 1)
    {
        while (num % divisor == 0)
        {
            list_add(factors, malloc(sizeof(unsigned long *)));
            *((unsigned long *)factors->tail->content) = divisor;
            num /= divisor;
        }
        divisor++;
        /* Skip even numbers greater than 2 */
        if (divisor > 2 && divisor % 2 == 0)
            divisor++;
    }

    return (factors);
}