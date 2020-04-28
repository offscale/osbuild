#include <stdio.h>
#include <stdlib.h>

#include "version.h"
#include "build_deps.h"

int main() {
    const char *distribution = get_distribution();
    printf("Running on %s\n", distribution);
    return osbuild_is_installed(distribution)? osbuild_install_build_dependencies(distribution) : EXIT_SUCCESS;
}
