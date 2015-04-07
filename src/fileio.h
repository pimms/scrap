#ifndef FILEIO_H__
#define FILEIO_H__

#include <stdio.h>

char* file_get_line(FILE *fp);
char* file_get_stripped(FILE *fp);

#endif /* __PANG_FILEIO_H__ */
