#include <stddef.h>

/**
 * _strlen - calculates the length of a string.
 * @src: pointer to the first character of the string.
 *
 * Return: size_t length of the string.
 */
size_t _strlen(const char *src)
{
    size_t len = 0;
    
    for (; src[len] != '\0'; len++);
    
    return len;
}

int _strcoll(const char *s1, const char *s2, option_t option)
{
    int result;
    
    if (option & CASE_INSENSITIVE) {
        result = strcasecmp(s1, s2);
    } else {
        result = strcmp(s1, s2);
    }
    
    if (option & REVERSE) {
        return -result;
    } else {
        return result;
    }
}