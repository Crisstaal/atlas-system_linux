#include <Python.h>
#include <stdio.h>

/**
 * print_python_bytes - prints information about a Python bytes object
 * @p: pointer to the Python object
 */
void print_python_bytes(PyObject *p)
{
    ssize_t size, i;
    
    printf("[.] bytes object info\n");
    if (!PyBytes_Check(p))
    {
        printf("  [ERROR] Invalid Bytes Object\n");
        return;
    }

    size = PyBytes_Size(p);
    printf("  size: %ld\n", size);
    printf("  trying string: %s\n", PyBytes_AsString(p));

    size = size < 10 ? size : 10;
    printf("  first %ld bytes: ", size);
    for (i = 0; i < size; i++)
    {
        printf("%02hhx", PyBytes_AsString(p)[i]);
        if (i + 1 != size)
            printf(" ");
    }
    printf("\n");
}

/**
 * print_python_list - prints information about a Python list object
 * @p: pointer to the Python object
 */
void print_python_list(PyObject *p)
{
    ssize_t size, i;

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
        PyObject *item = PyList_GetItem(p, i);

        printf("Element %ld: %s\n", i, Py_TYPE(item)->tp_name);
        if (PyBytes_Check(item))
        {
            print_python_bytes(item);
        }
    }
}