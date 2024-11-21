#include <stdio.h>
#include <stdlib.h>
#include <Python.h>
#include <time.h>  // Include time.h for struct timespec

/**
 * print_python_int - Prints Python integer info
 * @p: Python object
 */
void print_python_int(PyObject *p)
{
    unsigned long number = 0;
    ssize_t i = 0, size = 0, negative = 0;
    int shift = 0;

    /** Check if the object is a valid Python integer **/
    if (!PyLong_Check(p))
    {
        printf("Invalid Int Object\n");
        return;
    }
    size = ((PyVarObject *)p)->ob_size;
    negative = size < 0;
    size = negative ? -size : size;

   if (size == 3 && (((PyLongObject *)p)->ob_digit[2] > 0xf || size > 3))
	{
		printf("C unsigned long int overflow\n");
		return;
	}

    /** Iterate through the digits of the Python integer **/
    for (i = 0; i < size; i++)
    {
        shift = PyLong_SHIFT * i;
        unsigned long sub =
            ((unsigned long)((PyLongObject *)p)->ob_digit[i]) * (1UL << (shift));
        number += sub;
    }

    /** Print the number, handling the sign **/
    if (negative)
        printf("-");
    printf("%lu\n", number);
}