#ifndef OSBUILD_BSD_H
#define OSBUILD_BSD_H

#if !defined(WIN32) && !defined(_WIN32) && !defined(__WIN32__) && !defined(__NT__)
#include <sys/param.h>
#endif

#if defined(__FreeBSD__) || defined(__NetBSD__)

#include <stddef.h>
#include <stdbool.h>
#include <unistd.h>

inline bool osbuild_for_bsd_is_installed() {
    return access("/usr/bin/cc", F_OK) == 0 && access("/usr/bin/ld", F_OK) == 0
           && access("/usr/bin/make", F_OK) == 0 && access("/usr/lib/libc.a", F_OK) == 0;
}

#endif
#endif
