#include "hls.h"
#include <stddef.h>


 /* swap - swaps two files
 * @a: pointer
 * @b: pointer
 * Return: void
 */
void swap(file_t **a, file_t **b)
{
	file_t *tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

/**
 * _quicksort - sorts an array
 * @array: array
 * @size: length
 */
void _quicksort(file_t **array, size_t size)
{
	int pivot;
	if (size > 1)
	{
		pivot = partition(array, 0, size - 1);

		_quicksort(array, pivot);
		_quicksort(array + pivot + 1, size - pivot - 1);
	}
}

/**
 * _strcoll - handles the special options
 * @s1: pointer
 * @s2: pointer
 *
 * Return: 0 accordingly
 */
int _strcoll(char *s1, char *s2)
{
	return (_strcmp(s1, s2));
}
/**
 * _strlen - length the string.
 * @src: pointer
 * 
 * Return: length of the string
 */
size_t _strlen(const char *src)
{
	size_t len = 0;
	while (*src++)
		len++;
	return (len);
}

/**
 * _strcmp - compares strings
 * @s1: pointer
 * @s2: pointer
 *
 * Return: 0 accordingly
 */
int _strcmp(char *s1, char *s2)
{
	int i = 0;

	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}