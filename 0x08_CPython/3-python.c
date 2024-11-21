#include <Python.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * print_python_float - Prints information about a Python float object
 * @p: Pointer to Python object
 */
void print_python_float(PyObject *p)
{
    char *str;

    setbuf(stdout, NULL);
    printf("[.] float object info\n");
    if (!PyFloat_Check(p))
    {
        printf("  [ERROR] Invalid Float Object\n");
        return;
    }

    /* Use PyFloat_AS_DOUBLE to extract the float value */
    str = PyOS_double_to_string(PyFloat_AS_DOUBLE(p), 'r', 16, 0, NULL);
    printf("  value: %s%s\n", str, strchr(str, '.') ? "" : ".0");
    PyMem_Free(str); /* Free memory allocated by PyOS_double_to_string */
}

/**
 * print_python_bytes - Prints information about a Python bytes object
 * @p: Pointer to Python object
 */
void print_python_bytes(PyObject *p)
{
    ssize_t size, i;

    setbuf(stdout, NULL);
    printf("[.] bytes object info\n");
    if (!PyBytes_Check(p))
    {
        printf("  [ERROR] Invalid Bytes Object\n");
        return;
    }

    size = PyBytes_Size(p);
    printf("  size: %ld\n", size);
    printf("  trying string: %s\n", PyBytes_AS_STRING(p));

    /* Print up to the first 10 bytes */
    printf("  first %ld bytes: ", size < 10 ? size + 1 : 10);
    for (i = 0; i < size && i < 10; i++)
        printf("%02hhx%c", (unsigned char)PyBytes_AS_STRING(p)[i], i + 1 == 10 || i + 1 == size ? '\n' : ' ');
}

/**
 * print_python_list - Prints information about a Python list object
 * @p: Pointer to Python object
 */
void print_python_list(PyObject *p)
{
    ssize_t size, i;
    PyObject *item;

    setbuf(stdout, NULL);
    printf("[*] Python list info\n");
    if (!PyList_Check(p))
    {
        printf("  [ERROR] Invalid List Object\n");
        return;
    }

    size = PyList_Size(p);
    printf("[*] Size of the Python List = %ld\n", size);
    printf("[*] Allocated = %ld\n", ((PyListObject *)p)->allocated);

    for (i = 0; i < size; i++)
    {
        item = PyList_GetItem(p, i);
        printf("Element %ld: %s\n", i, Py_TYPE(item)->tp_name);

        /* Check for specific types to print additional info */
        if (PyBytes_Check(item))
            print_python_bytes(item);
        else if (PyFloat_Check(item))
            print_python_float(item);
    }
}