#include <Python.h>
#include <stdio.h>

/**
 * print_python_list - Prints information about a Python list.
 *                     Displays the size, allocated space, and element types.
 *
 * @p: PyObject* pointer representing a Python list object.
 *
 */
void print_python_list(PyObject *p)
{
    if (!PyList_Check(p)) {
        fprintf(stderr, "Provided PyObject is not a Python list.\n");
        return;
    }

    Py_ssize_t size = Py_SIZE(p);
    Py_ssize_t allocated = ((PyListObject *)p)->allocated;

    printf("[*] Python list info\n");
    printf("[*] Size of the Python List = %ld\n", size);
    printf("[*] Allocated = %ld\n", allocated);
    for (Py_ssize_t i = 0; i < size; i++) {
        PyObject *item = PyList_GetItem(p, i);
        printf("Element %ld: %s\n", i, Py_TYPE(item)->tp_name);
    }
}