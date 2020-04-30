extern struct DocoptArgs {
    /* options without arguments */
    size_t check;
    size_t distribution;
    size_t help;
    size_t no_cache;
    size_t no_check;
    size_t no_update;
    size_t version;

    /* special */
    const char *usage_pattern;
    const char *help_message;
};
