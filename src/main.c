#include <stdio.h>

#include "version.h"
#include "build_deps.h"

int main() {
    const char* distribution = get_distribution();
    printf("Running on %s\n", distribution);
    return install_build_dependencies(distribution);
}
