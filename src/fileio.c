#include "fileio.h"
#include <string.h>
#include <malloc.h>


static void strstrip(char *line);


char*
file_get_line(FILE *fp)
{
    const int len_incr = 80;
    int len = len_incr;
    char *buf = malloc(len);
    int read = 0;
    int ch = 0;

    while ((ch = fgetc(fp)) != EOF) {
        buf[read] = ch;
        read++;

        if (ch == '\n')
            break;

        if (read >= len) {
            len += len_incr;
            buf = realloc(buf, len);
        }
    }

	if (!read) {
		free(buf);
		return NULL;
	}

    buf[read] = 0;
    return buf;
}

char*
file_get_stripped(FILE *fp)
{
    char *line = file_get_line(fp);

    if (line)
        strstrip(line);

    return line;
}

void
strstrip(char *line)
{
    const int line_len = strlen(line);
    char *base = line;
    char *head = line;

    while (*base) {
        while (*head == ' ' || *head == '\t')
            head++;

        if (head - base > 1) {
            int remaining = line_len - (head - base + 1);
            memcpy(base + 1, head, remaining);
            base++;
            *base = ' ';
            head[remaining] = 0;
        } else if (*base == '\t')
			*base = ' ';

        base++;
        head = base;
    }
}
