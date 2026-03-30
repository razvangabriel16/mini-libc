#include <internal/io.h>
#include <internal/syscall.h>
#include <internal/types.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

int puts(const char* s){
    int len = write(1, s, strlen(s));
    int after = write(1, "\n", 1);
    if (len < 0 || after < 0) return -1;
    return 1;
}
