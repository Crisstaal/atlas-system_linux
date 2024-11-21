#include <Python.h>
#include <stdio.h>
#include <limits.h>

void print_python_int(PyObject *p)
{
    unsigned long int value;
    long sign;

    if (!PyLong_Check(p))
    {
        printf("Invalid Int Object\n");
        return;
    }

    sign = PyLong_Sign(p);
    
    if (sign == -1)
    {
        /*Handle negative value*/
        value = (unsigned long int)(-PyLong_AsLong(p));
    }
    else
    {
        /*Handle positive value*/
        value = (unsigned long int)PyLong_AsLong(p);
    }

    if (value > ULONG_MAX)
    {
        printf("C unsigned long int overflow\n");
        return;
    }

    printf("%lu\n", value);
}