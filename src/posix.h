#ifndef OSBUILD_POSIX_H
#define OSBUILD_POSIX_H

#if !defined(WIN32) && !defined(_WIN32) && !defined(__WIN32__) && !defined(__NT__)

#include <stdbool.h>

extern bool exists(const char *);
extern int execute_bin(const char *const *const);

#endif
#endif //OSBUILD_POSIX_H
