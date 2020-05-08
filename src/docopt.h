#ifndef DOCOPT_DOCOPT_H
#define DOCOPT_DOCOPT_H

#include <stddef.h>

struct DocoptArgs {
    /* options without arguments */
    size_t check;
    size_t help;
    size_t no_cache;
    size_t no_check;
    size_t no_update;
    size_t version;
    /* options with arguments */
    char *distribution;
    /* special */
    const char *usage_pattern;
    const char *help_message[21];
};

struct DocoptArgs docopt(size_t,char *[],bool,const char *);

#endif
