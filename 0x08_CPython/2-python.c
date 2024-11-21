#include <Python.h>
#include <stdio.h>

/**
 * print_python_list - Prints information about a Python list.
 *                     Displays size, allocated space, and element types.
 *
 * @p: PyObject* pointer representing a Python list.
 */
void print_python_list(PyObject *p)
{
    if (!PyList_Check(p)) {
        fprintf(stderr, "Provided PyObject is not a Python list.\n");
        return;
    }

    Py_ssize_t size = ((PyVarObject *)p)->ob_size;
    Py_ssize_t allocated = ((PyListObject *)p)->allocated;

    printf("[*] Python list info\n");
    printf("[*] Size of the Python List = %ld\n", size);
    printf("[*] Allocated = %ld\n", allocated);

    for (Py_ssize_t i = 0; i < size; i++) {
        PyObject *item = ((PyListObject *)p)->ob_item[i];
        const char *type_name = item->ob_type->tp_name;
        printf("Element %ld: %s\n", i, type_name);
    }
}

/**
 * print_python_bytes - Prints information about a Python bytes object.
 *                      Displays size, content in string form, and first
 *                      up to 10 bytes in hexadecimal.
 *
 * @p: PyObject* pointer representing a Python bytes object.
 */
void print_python_bytes(PyObject *p)
{
    if (!PyBytes_Check(p)) {
        fprintf(stderr, "Provided PyObject is not a Python bytes object.\n");
        return;
    }

    Py_ssize_t size = ((PyVarObject *)p)->ob_size;
    char *bytes_str = ((PyBytesObject *)p)->ob_sval;
    Py_ssize_t display_size = size < 10 ? size : 10;

    printf("[.] bytes object info\n");
    printf("  size: %ld\n", size);
    printf("  trying string: %s\n", bytes_str);
    printf("  first %ld bytes:", display_size);
    for (Py_ssize_t i = 0; i < display_size; i++) {
        printf(" %02x", (unsigned char)bytes_str[i]);
    }
    printf("\n");
}
