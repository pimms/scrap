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
        if (ch == '\n')
            break;

        buf[read] = ch;
        read++;

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
    char *base = line;
    char *head = line;

	printf("--- strstrip\n");
	printf("    '%s'\n", line);

    while (*base) {
        while (*head == ' ' || *head == '\t')
            head++;

        if (head - base > 0) {
			printf("hd: '%s'\n", head);

            int head_len = strlen(head) + 1;

			int offset = (base==line || !*head ? 0 : 1);
            memmove(base + offset, head, head_len);

			*base = ' ';
            base++;
			printf("ln: '%s'\n", line);
        }

		if (*base == '\t')
			*base = ' ';

        base++;
        head = base;
    }
}
