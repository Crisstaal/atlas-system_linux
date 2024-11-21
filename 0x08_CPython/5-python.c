#include <Python.h>
#include <stdio.h>

/**
 * print_python_int - Prints a Python integer object.
 * @p: Pointer to a Python object.
 *
 * Description:
 * - Converts the Python integer to a C unsigned long integer.
 * - Prints the value or an error message if the object is invalid.
 */
void print_python_int(PyObject *p)
{
    unsigned long int result = 0;
    Py_ssize_t size, i;
    unsigned long current_digit;
    PyLongObject *long_obj;

    setbuf(stdout, NULL);
    printf("[.] integer object info\n");

    /* Check if the object is a valid Python integer */
    if (!PyLong_Check(p))
    {
        printf("  [ERROR] Invalid Integer Object\n");
        return;
    }

    long_obj = (PyLongObject *)p;
    size = Py_ABS(long_obj->ob_base.ob_size);

    /* Check if the number is too large for a C unsigned long */
    if (size > (unsigned long)(sizeof(unsigned long) * 8 / PyLong_SHIFT))
    {
        printf("  C unsigned long int overflow\n");
        return;
    }

    /* Convert Python integer digits to C unsigned long */
    for (i = 0; i < size; i++)
    {
        current_digit = long_obj->ob_digit[i];
        result += current_digit << (i * PyLong_SHIFT);
    }

    /* Print the resulting unsigned long */
    if (long_obj->ob_base.ob_size < 0)
        printf("  -%lu\n", result);
    else
        printf("  %lu\n", result);
}