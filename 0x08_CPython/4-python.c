#include <Python.h>
#include <stdio.h>

/**
 * print_python_string - Prints information about a Python string object
 * @p: Pointer to a Python object
 */
void print_python_string(PyObject *p)
{
    setbuf(stdout, NULL);
    printf("[.] string object info\n");

    /* Verify if the object is a valid Python string */
    if (!PyUnicode_Check(p))
    {
        printf("  [ERROR] Invalid String Object\n");
        return;
    }

    /* Get the string length and representation type */
    Py_ssize_t length = PyUnicode_GET_LENGTH(p);
    const char *kind = PyUnicode_IS_COMPACT_ASCII(p) ? "compact ascii" : "compact unicode object";

    printf("  type: %s\n", kind);
    printf("  length: %ld\n", length);

    /* Convert Python string to UTF-8 for printing */
    const char *utf8_str = PyUnicode_AsUTF8(p);
    if (utf8_str == NULL)
    {
        printf("  [ERROR] Unable to decode string\n");
        return;
    }

    printf("  value: %s\n", utf8_str);
}