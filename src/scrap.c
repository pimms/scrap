#include <stdlib.h>
#include <stdio.h>

#include "fileio.h"

int main(int argc, char **argv)
{
    if (argc == 1) {
        return 1;
    }

    FILE *fp = fopen(argv[1], "r");
    char *buf = file_get_stripped(fp);

    while (buf) {
		free(buf);
        printf("'%s'\n", buf);
        buf = file_get_stripped(fp);
    }

    fclose(fp);
    return 0;
}
