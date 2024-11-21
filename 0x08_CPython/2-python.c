#include <Python.h>
#include <stdio.h>

/**
 * print_python_bytes - prints information about a Python bytes object
 * @p: pointer to the Python object
 */
void print_python_bytes(PyObject *p) {
    if (!PyBytes_Check(p)) {
        printf("[.] bytes object info\n  [ERROR] Invalid Bytes Object\n");
        return;
    }

    Py_ssize_t size = PyBytes_Size(p);
    unsigned char *bytes = (unsigned char *)PyBytes_AsString(p);

    printf("[.] bytes object info\n");
    printf("  size: %zd\n", size);
    printf("  trying string: %s\n", bytes);

    printf("  first 6 bytes: ");
    for (int i = 0; i < 6 && i < size; i++) {
        printf("%02x ", bytes[i]);
    }
    for (int i = size; i < 6; i++) {
        printf("00 ");
    }
    printf("\n");
}

/**
 * print_python_list - prints information about a Python list object
 * @p: pointer to the Python object
 */
void print_python_list(PyObject *p) {
    if (!PyList_Check(p)) {
        printf("[.] list object info\n  [ERROR] Invalid List Object\n");
        return;
    }

    Py_ssize_t size = PyList_Size(p);
    printf("[*] Python list info\n");
    printf("[*] Size of the Python List = %zd\n", size);
    printf("[*] Allocated = %zd\n", size); // Assume the allocated size is the same for simplicity

    for (Py_ssize_t i = 0; i < size; i++) {
        PyObject *item = PyList_GetItem(p, i);  // Retrieve each item in the list
        if (item == NULL) {
            continue; // Handle cases where the item is NULL
        }

        printf("Element %zd: ", i);

        if (PyBytes_Check(item)) {
            print_python_bytes(item); // Print bytes object info if item is a bytes object
        } else if (PyLong_Check(item)) {
            printf("int\n");
        } else if (PyFloat_Check(item)) {
            printf("float\n");
        } else if (PyTuple_Check(item)) {
            printf("tuple\n");
        } else if (PyList_Check(item)) {
            printf("list\n");
        } else if (PyUnicode_Check(item)) {
            printf("str\n");
        } else {
            printf("unknown\n");
        }
    }
}
