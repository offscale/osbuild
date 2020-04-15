#ifndef OSBUILD_POSIX_H
#define OSBUILD_POSIX_H

#if defined (__unix__) || (defined (__APPLE__) && defined (__MACH__))

#if defined (__APPLE__) && defined (__MACH__)
#define ENOPKG 65
#endif

#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "build_deps.h"

static int exists(const char *);
int execute_bin(const char *const *const);

#endif
#endif //OSBUILD_POSIX_H
