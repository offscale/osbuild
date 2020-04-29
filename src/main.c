#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "version.h"
#include "build_deps.h"

int main() {
    const bool update = getenv("NO_UPDATE") == NULL;
    const char *distribution = get_distribution();
    printf("Running on %s\n", distribution);
    return osbuild_is_installed(distribution)? EXIT_SUCCESS : osbuild_install_build_dependencies(distribution, update);
}
