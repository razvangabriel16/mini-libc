// SPDX-License-Identifier: BSD-3-Clause

#include <time.h>
#include <internal/syscall.h>
#include <stdarg.h>
#include <errno.h>

unsigned int sleep(unsigned int seconds){
    struct timespec req = {
        .tv_sec = seconds,
        .tv_nsec = 0
    };
    struct timespec rem = {
        .tv_sec = 0,
        .tv_nsec = 0
    };
    if (nanosleep(&req, &rem) == -1){
        if (errno == EINTR){
            return rem.tv_sec;
        }
        return -1;
    }
    return 0;
}

