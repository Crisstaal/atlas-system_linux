#include <stdio.h>
#include <stdlib.h>

/**
 * print_python_bytes - prints a python bytes object
 * @p: pointer to python bytes object
 */
void print_python_bytes(PyObject *p)
{
    ssize_t sz, i = 0;

    printf("[.] bytes object info\n");
    if (p->ob_type != &PyBytes_Type)
    {
        printf("  [ERROR] Invalid Bytes Object\n");
        return;
    }
    sz = ((PyVarObject *)p)->ob_size;
    printf("  size: %ld\n", sz);
    printf("  trying string: %s\n", ((PyBytesObject *)p)->ob_sval);
    sz = sz < 9 ? sz + 1 : 10;
    printf("  first %ld bytes: ", sz);
    for (i = 0; i < sz; i++)
        printf("%02hhx%c",
            ((PyBytesObject *)p)->ob_sval[i], i + 1 == sz ? '\n' : ' ');
}

/**
 * print_python_list - prints info about a python list
 * @p: pointer to python object struct
 */
void print_python_list(PyObject *p)
{
    ssize_t sz = ((PyVarObject *)p)->ob_size, i = 0;

    printf("%s", "[*] Python list info\n");
    printf("[*] Size of the Python List = %ld\n", sz);
    printf("[*] Allocated = %ld\n", ((PyListObject *)p)->allocated);
    for (i = 0; i < sz; i++)
    {
        PyObject *pobj = ((PyListObject *)p)->ob_item[i];

        printf("Element %ld: %s\n", i, pobj->ob_type->tp_name);
        if (pobj->ob_type == &PyBytes_Type)
            print_python_bytes(pobj);
    }
}

/**
 * print_python_int - prints a Python integer object
 * @p: pointer to Python object
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