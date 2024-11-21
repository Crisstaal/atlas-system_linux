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
 * print_python_bytes - Prints information about a Python bytes object.
 *                      Displays size, content in string form, and first
 *                      up to 10 bytes in hexadecimal.
 *
 * @p: PyObject* pointer representing a Python bytes object.
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