#include <stdio.h>
#include "_getline.h"
#include <string.h>

#define OPEN_MAX 255
static filebuf_t fds[OPEN_MAX];

/**
 * _fgetchar - getchar
 * @fd: file dscriptor
 * Return: value of char read or -1 on EOF
 */
ssize_t _fgetchar(const int fd)
{
	filebuf_t *fb;
	int i;

	if (fd == -1)
	{
		for (i = 3; i < OPEN_MAX; ++i)
		{
			fb = &fds[i];
			if (fb->been_opened)
			{
				dbg_printf(BLUE "%d had been opened. zeroing out now..\n" RESET, i);
				memset(fb, 0, sizeof(*fb));
				dbg_printf(RED "buf contents: %s\n" RESET, fb->buf);
			}
		}
		return (EOF);
    }
    
	fb = &fds[fd];
	fb->been_opened = 1;
	if (fb->n == 0)
	{
		fb->n = read(fd, fb->buf, READ_SIZE);
		fb->bufp = fb->buf;
	}
	return ((--fb->n >= 0) ? (unsigned char)*(fb->bufp)++ : EOF);
}

/**
 * _getline - get line
 * @fd: file descriptor
 * Return: NULL on error or EOF
 */
char *_getline(int fd) {
   char *buf, 
   char *ptr, 
   char *endptr, c;

	if (fd == -1)
	{
		_fgetchar(-1);
		return (NULL);
	}
	else
	{
		buf = malloc(LINEBUF_SIZE);
		if (buf == NULL)
			return (NULL);
		ptr = buf;
		endptr = ptr + LINEBUF_SIZE;
		while ((c = _fgetchar(fd)) >= 0)
		{
			*ptr++ = c;
			if (c == '\n')
			{
				*(--ptr) = '\0';
				return (buf);
			}
			if (ptr == endptr)
			{
				long int diff = endptr - buf;

				dbg_printf(BLUE "old size of buf: %ld\n" RESET, diff);
				buf = realloc(buf, diff + LINEBUF_SIZE);
				ptr = buf + diff;
				endptr = ptr + LINEBUF_SIZE;
			}
		}
		if (ptr - buf != 0)
			*ptr = '\0';
		else
		{
			free(buf);
			return (NULL);
		}
	}
	return (buf);
}