#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "build_deps.h"
#include "docopt.h"
#include "distribution.h"

/*
#include "version.h"
*/
#define VERSION "0.0.1"

int main(int argc, char *argv[]) {
    const char *distribution = get_distribution();
    printf("Running on %s\n", distribution);

    struct DocoptArgs args = docopt(argc, argv, /* help */ 1, /* version */ VERSION);
    /* TODO: Ensure environment variables don't take priority over CLI arguments */
    const char *no_cache = getenv("NO_CACHE");
    if (no_cache != NULL && args.no_cache == 0) args.no_cache = (bool)no_cache;
    const char *no_update = getenv("NO_UPDATE");
    if (no_update != NULL && args.no_update == 0) args.no_update = (bool)no_update;
    const char *no_check = getenv("NO_CHECK");
    if (no_check != NULL && args.no_check == 0) args.check = (bool)no_check;
    const char *check = getenv("CHECK");
    if (check != NULL && args.check == 0) args.check = (bool)check;

    /*printf("Commands\n");
    printf("    args.no_cache:\t\t%s\n", args.no_cache ? "true" : "false");
    printf("    args.no_check:\t\t%s\n", args.no_check ? "true" : "false");
    printf("    args.no_update:\t\t%s\n", args.no_update ? "true" : "false");
    printf("    args.check:\t\t\t%s\n", args.check ? "true" : "false");
    printf("    args.distribution:\t%s\n", args.distribution);*/

    if (args.distribution == NULL)
        args.distribution = (char*)distribution;
    else
        printf("Acting as if distribution is %s\n", args.distribution);

    if (args.no_check)
        return osbuild_install_build_dependencies(&args);
    else if (args.check)
        return osbuild_is_installed(args.distribution);
    else
        return osbuild_is_installed(args.distribution) ?
               EXIT_SUCCESS : osbuild_install_build_dependencies(&args);
}
