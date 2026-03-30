/* SPDX-License-Identifier: BSD-3-Clause */

#ifndef __TIME_H__
#define __TIME_H__	1

#ifdef __cplusplus
extern "C" {
#endif

struct timespec {
    long tv_sec;    /* seconds */
    long tv_nsec;   /* nanoseconds */
};

int nanosleep(const struct timespec *duration,
                     struct timespec *rem);

#ifdef __cplusplus
}
#endif

#endif
