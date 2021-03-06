#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include "docopt.h"

struct Command {
    const char *name;
    bool value;
};

struct Argument {
    const char *name;
    char *value;
    char **array;
};

struct Option {
    const char *oshort;
    const char *olong;
    bool argcount;
    bool value;
    char *argument;
};

struct Elements {
    size_t n_commands;
    size_t n_arguments;
    size_t n_options;
    struct Command *commands;
    struct Argument *arguments;
    struct Option *options;
};


/*
 * Tokens object
 */

struct Tokens {
    size_t argc;
    char **argv;
    size_t i;
    char *current;
};

const char usage_pattern[] =
"Usage:\n"
"  osbuild --check\n"
"  osbuild --no-check\n"
"  osbuild --no-cache\n"
"  osbuild --no-update\n"
"  osbuild --distribution=<d>\n"
"  osbuild --help\n"
"  osbuild --version";

struct Tokens tokens_new(size_t argc, char **argv) {
    struct Tokens ts;
    ts.argc = argc;
    ts.argv = argv;
    ts.i = 0;
    ts.current = argv[0];
    return ts;
}

struct Tokens* tokens_move(struct Tokens *ts) {
    if (ts->i < ts->argc) {
        ts->current = ts->argv[++ts->i];
    }
    if (ts->i == ts->argc) {
        ts->current = NULL;
    }
    return ts;
}


/*
 * ARGV parsing functions
 */

size_t parse_doubledash(struct Tokens *ts, struct Elements *elements) {
    /*
    size_t n_commands = elements->n_commands;
    size_t n_arguments = elements->n_arguments;
    Command *commands = elements->commands;
    Argument *arguments = elements->arguments;

    not implemented yet
    return parsed + [Argument(None, v) for v in tokens]
    */
    return 0;
}

size_t parse_long(struct Tokens *ts, struct Elements *elements) {
    size_t i;
    size_t len_prefix;
    size_t n_options = elements->n_options;
    char *eq = strchr(ts->current, '=');
    struct Option *option;
    struct Option *options = elements->options;

    len_prefix = (eq-(ts->current))/sizeof(char);
    for (i=0; i < n_options; i++) {
        option = &options[i];
        if (!strncmp(ts->current, option->olong, len_prefix))
            break;
    }
    if (i == n_options) {
        /* TODO '%s is not a unique prefix */
        fprintf(stderr, "%s is not recognized\n", ts->current);
        return 1;
    }
    tokens_move(ts);
    if (option->argcount) {
        if (eq == NULL) {
            if (ts->current == NULL) {
                fprintf(stderr, "%s requires argument\n", option->olong);
                return 1;
            }
            option->argument = ts->current;
            tokens_move(ts);
        } else {
            option->argument = eq + 1;
        }
    } else {
        if (eq != NULL) {
            fprintf(stderr, "%s must not have an argument\n", option->olong);
            return 1;
        }
        option->value = true;
    }
    return 0;
}

size_t parse_shorts(struct Tokens *ts, struct Elements *elements) {
    char *raw;
    size_t i;
    size_t n_options = elements->n_options;
    struct Option *option;
    struct Option *options = elements->options;

    raw = &ts->current[1];
    tokens_move(ts);
    while (raw[0] != '\0') {
        for (i=0; i < n_options; i++) {
            option = &options[i];
            if (option->oshort != NULL && option->oshort[1] == raw[0])
                break;
        }
        if (i == n_options) {
            /* TODO -%s is specified ambiguously %d times */
            fprintf(stderr, "-%c is not recognized\n", raw[0]);
            return 1;
        }
        raw++;
        if (!option->argcount) {
            option->value = true;
        } else {
            if (raw[0] == '\0') {
                if (ts->current == NULL) {
                    fprintf(stderr, "%s requires argument\n", option->oshort);
                    return 1;
                }
                raw = ts->current;
                tokens_move(ts);
            }
            option->argument = raw;
            break;
        }
    }
    return 0;
}

size_t parse_argcmd(struct Tokens *ts, struct Elements *elements) {
    size_t i;
    size_t n_commands = elements->n_commands;
    /* size_t n_arguments = elements->n_arguments; */
    struct Command *command;
    struct Command *commands = elements->commands;
    /* Argument *arguments = elements->arguments; */

    for (i=0; i < n_commands; i++) {
        command = &commands[i];
        if (!strcmp(command->name, ts->current)){
            command->value = true;
            tokens_move(ts);
            return 0;
        }
    }
    /* not implemented yet, just skip for now
       parsed.append(Argument(None, tokens.move())) */
    /*fprintf(stderr, "! argument '%s' has been ignored\n", ts->current);
    fprintf(stderr, "  '");
    for (i=0; i<ts->argc ; i++)
        fprintf(stderr, "%s ", ts->argv[i]);
    fprintf(stderr, "'\n");*/
    tokens_move(ts);
    return 0;
}

size_t parse_args(struct Tokens *ts, struct Elements *elements) {
    size_t ret;

    while (ts->current != NULL) {
        if (strcmp(ts->current, "--") == 0) {
            ret = parse_doubledash(ts, elements);
            if (!ret) break;
        } else if (ts->current[0] == '-' && ts->current[1] == '-') {
            ret = parse_long(ts, elements);
        } else if (ts->current[0] == '-' && ts->current[1] != '\0') {
            ret = parse_shorts(ts, elements);
        } else
            ret = parse_argcmd(ts, elements);
        if (ret) return ret;
    }
    return 0;
}

size_t elems_to_args(struct Elements *elements, struct DocoptArgs *args, const bool help,
                     const char *version){
    struct Command *command;
    struct Argument *argument;
    struct Option *option;
    size_t i, j;

    /* fix gcc-related compiler warnings (unused) */
    (void)command;
    (void)argument;

    /* options */
    for (i=0; i < elements->n_options; i++) {
        option = &elements->options[i];
        if (help && option->value && !strcmp(option->olong, "--help")) {
            for(j=0; j < 21; j++)
                puts(args->help_message[j]);
            return 1;
        } else if (version && option->value &&
                   !strcmp(option->olong, "--version")) {
            printf("%s\n", version);
            return 1;
        } else if (!strcmp(option->olong, "--check")) {
            args->check = option->value;
        } else if (!strcmp(option->olong, "--help")) {
            args->help = option->value;
        } else if (!strcmp(option->olong, "--no-cache")) {
            args->no_cache = option->value;
        } else if (!strcmp(option->olong, "--no-check")) {
            args->no_check = option->value;
        } else if (!strcmp(option->olong, "--no-update")) {
            args->no_update = option->value;
        } else if (!strcmp(option->olong, "--version")) {
            args->version = option->value;
        } else if (!strcmp(option->olong, "--distribution")) {
            if (option->argument)
                args->distribution = option->argument;
        }
    }
    /* commands */
    for (i=0; i < elements->n_commands; i++) {
        command = &elements->commands[i];
    }
    /* arguments */
    for (i=0; i < elements->n_arguments; i++) {
        argument = &elements->arguments[i];
    }
    return 0;
}


/*
 * Main docopt function
 */

struct DocoptArgs docopt(size_t argc, char *argv[], const bool help, const char *version) {
    struct DocoptArgs args = {
        0, 0, 0, 0, 0, 0, NULL,
        usage_pattern,
        { "osbuild: installs compiler and build tools for your platform.",
          "E.g., on Ubuntu it will run `apt-get install -y build-essential.`",
          "",
          "Usage:",
          "  osbuild --check",
          "  osbuild --no-check",
          "  osbuild --no-cache",
          "  osbuild --no-update",
          "  osbuild --distribution=<d>",
          "  osbuild --help",
          "  osbuild --version",
          "",
          "Options:",
          "  -h --help           Show this screen.",
          "  --version           Show version.",
          "  --check             Only check if installed, don't install anything.",
          "  --no-check          Optimisation argument, set if you know that nothing is installed.",
          "  --no-cache          Skip cache. Known to work with `apk` (Alpine Linux).",
          "  --no-update         Skip update. E.g., `apk update`, `apt-get update`.",
          "  --distribution=<d>  Operating System distribution. E.g., 'OpenIndiana', 'alpine', 'macOS'. Will derive when unspecified.",
          ""}
    };
    struct Tokens ts;
    struct Command commands[] = {NULL
    };
    struct Argument arguments[] = {NULL
    };
    struct Option options[] = {
        {NULL, "--check", 0, 0, NULL},
        {"-h", "--help", 0, 0, NULL},
        {NULL, "--no-cache", 0, 0, NULL},
        {NULL, "--no-check", 0, 0, NULL},
        {NULL, "--no-update", 0, 0, NULL},
        {NULL, "--version", 0, 0, NULL},
        {NULL, "--distribution", 1, 0, NULL}
    };
    struct Elements elements;
    elements.n_commands = 0;
    elements.n_arguments = 0;
    elements.n_options = 7;
    elements.commands = commands;
    elements.arguments = arguments;
    elements.options = options;

    ts = tokens_new(argc, argv);
    if (parse_args(&ts, &elements))
        exit(EXIT_FAILURE);
    if (elems_to_args(&elements, &args, help, version))
        exit(EXIT_SUCCESS);
    return args;
}
