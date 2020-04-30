#ifndef OSBUILD_BUILD_DEPS_H
#define OSBUILD_BUILD_DEPS_H

#include <stdbool.h>
#include "docopt.h"

extern inline int osbuild_install_build_dependencies(struct DocoptArgs);
extern inline bool osbuild_is_installed(const char*);

#endif
