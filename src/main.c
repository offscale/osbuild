#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "build_deps.h"
#include "docopt.c"
#include "distribution.h"
#define VERSION "0.0.1"
/*
#include "version.h"
*/

int main(int argc, char *argv[])
{
    DocoptArgs args = docopt(argc, argv, /* help */ 1, /* version */ VERSION);
    /* TODO: Ensure environment variables don't take priority over CLI arguments */
    const char *no_cache = getenv("NO_CACHE");
    if (no_cache != NULL && args.no_cache == 0) args.no_cache = (bool)no_cache;
    const char *no_update = getenv("NO_UPDATE");
    if (no_update != NULL && args.no_update == 0) args.no_update = (bool)no_update;
    const char *check = getenv("CHECK");
    if (check != NULL && args.check == 0) args.check = (bool)check;

    printf("Commands\n");
    printf("    args.no_cache == %s\n", args.no_cache ? "true" : "false");
    printf("    args.no_check == %s\n", args.no_check ? "true" : "false");
    printf("    args.no_update == %s\n", args.no_update ? "true" : "false");
    printf("    args.check == %s\n", args.check ? "true" : "false");

    const char *distribution = get_distribution();
    printf("Running on %s\n", distribution);
    return osbuild_is_installed(distribution)? EXIT_SUCCESS : osbuild_install_build_dependencies(distribution, args);
}
