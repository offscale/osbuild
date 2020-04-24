#ifndef OSBUILD_POSIX_H
#define OSBUILD_POSIX_H

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

#else

#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>

extern int exists(const char *);
extern int execute_bin(const char *const *const);

#endif
#endif //OSBUILD_POSIX_H
