#ifndef OSBUILD_BSD_H
#define OSBUILD_BSD_H

#if !defined(WIN32) && !defined(_WIN32) && !defined(__WIN32__) && !defined(__NT__)
#include <sys/param.h>
#endif

#if defined(__FreeBSD__) || defined(__NetBSD__)

#include <stddef.h>
#include <stdbool.h>

#include "posix.h"

inline bool osbuild_for_bsd_is_installed() {
    return exists("/usr/bin/cc") && exists("/usr/bin/ld")
           && exists("/usr/bin/make") && exists("/usr/lib/libc.a");
}

#endif
#endif
