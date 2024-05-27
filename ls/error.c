#include "hls.h"

/**
 * dbg_print_array - prints pathn
 * @array: the array
 * @size: number of elements
 */
void dbg_print_array(file_t **array, size_t size)
{
	size_t i = 0;

	while (array && i < size)
	{
		if (i > 0)
			printf(", ");
		printf("%s", array[i]->path);
		++i;
	}
	printf("\n");
}

/**
 * dbg_swap - swaps two values
 * @a: first value to swap
 * @b: second value to swap
 * @array: pointer to first element of initial array
 * @size: full size of initial array
 */
void dbg_swap(file_t **a, file_t **b, file_t **array, size_t size)
{
	/* for printing */
	static file_t **ptr;
	static size_t s;

	file_t *tmp = *a;

	if (*a != *b)
	{
		*a = *b;
		*b = tmp;
		if (!ptr)
			ptr = array, s = size;
		dbg_print_array(ptr, s);
	}
}


/**
  * dbg_print_binary - prints the binary representation
  * @r: unsigned char
  */
void dbg_print_binary(unsigned char r)
{
	int bits = 8;
	int bit_offset = sizeof(r) * bits;
	unsigned char bit = 1 << (bit_offset - 1);

	while (bits)
	{
		if (bits)
			(bit & r) ? putchar('1') : putchar('0');
		bit >>= 1;
		--bits;
	}
	putchar('\n');
}
/**
 * sort_subentries - sorts an array l
 * @directory: directory
 * @d_count: count
 */
void sort_subentries(file_t **directory, int d_count)
{
	_alphasort(dirs, (size_t)d_count);
}

void _alphasort(file_t **files, int file_count)
{
	_quicksort(files, (size_t)file_count);
}

/**