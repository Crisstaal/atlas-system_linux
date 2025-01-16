#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "list.h"

/* Function declarations */
list_t *initialize_prime_list(char const *s);
int find_prime_factors(list_t *prime_list, unsigned long *num);
int append_remaining_prime(list_t *prime_list, unsigned long num);

/**
 * prime_factors - Factorizes a given number into its prime factors
 * @s: A string representing the number to be factorized
 * Return: A pointer to the list containing the prime factors
 */
list_t *prime_factors(char const *s)
{
    unsigned long num;
    list_t *prime_list = initialize_prime_list(s);

    if (!prime_list)
        return (NULL); /* Return NULL if input is invalid or allocation fails */

    num = strtoul(s, NULL, 10);
    
    /* Add prime factors of the number */
    if (find_prime_factors(prime_list, &num) == -1)
    {
        list_destroy(prime_list, free);
        free(prime_list);
        return (NULL);
    }

    /* Add any remaining prime factor larger than sqrt(num) */
    if (append_remaining_prime(prime_list, num) == -1)
    {
        list_destroy(prime_list, free);
        free(prime_list);
        return (NULL);
    }

    return (prime_list); /* Return the list containing all prime factors */
}

/**
 * initialize_prime_list - Creates and initializes the list for prime factors
 * @s: String representation of the number to be factored
 * Return: A pointer to the initialized list or NULL if there's an error
 */
list_t *initialize_prime_list(char const *s)
{
    list_t *prime_list = malloc(sizeof(list_t));

    if (!prime_list || *s == '\0')
    {
        free(prime_list);
        return (NULL); /* Memory allocation failure or invalid input */
    }

    list_init(prime_list); /* Initialize the list */
    return (prime_list);
}

/**
 * find_prime_factors - Extracts prime factors of the given number and adds them to the list
 * @prime_list: The list to store the prime factors
 * @num: A pointer to the number being factorized
 * Return: 0 on success, -1 if memory allocation fails
 */
int find_prime_factors(list_t *prime_list, unsigned long *num)
{
    unsigned long prime = 2;
    unsigned long *temp;

    /* Loop through possible factors up to sqrt(num) */
    while (prime * prime <= *num)
    {
        while (*num % prime == 0)
        {
            temp = malloc(sizeof(unsigned long));
            if (!temp)
                return (-1); /* Memory allocation failure */

            *temp = prime;  /* Store the current prime factor */
            list_add(prime_list, temp);  /* Add to the prime list */
            *num /= prime; /* Reduce the number by the prime factor */
        }
        prime += 1 + (prime != 2); /* Skip even numbers after 2 */
    }
    return (0);
}

/**
 * append_remaining_prime - Adds the last prime factor if greater than sqrt(num)
 * @prime_list: The list to store the prime factors
 * @num: The remaining value after factorization
 * Return: 0 on success, -1 on failure (e.g., memory allocation error)
 */
int append_remaining_prime(list_t *prime_list, unsigned long num)
{
    unsigned long *temp;

    /* If num is still greater than 2, it's a prime factor */
    if (num >= 2)
    {
        temp = malloc(sizeof(unsigned long));
        if (!temp)
            return (-1); /* Memory allocation failure */

        *temp = num;  /* Store the remaining prime factor */
        list_add(prime_list, temp);  /* Add it to the prime list */
    }
    return (0);
}