#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "systemd.h"

const char* parse_id_from_os_release(const char* filename) {
    static char candidate[80]; /* What distribution is more than 80 characters?! */
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("File opening failed");
        return "";
    }

    bool correct_line = false;
    bool break_loop = false;
    int c;
    size_t i = 0;
    while ((c = fgetc(fp)) != EOF) {
        switch (c) {
            case '\n':
                if (correct_line) {
                    candidate[++i] = '\0';
                    break_loop = true; /* goodbye `goto`! */
                } else {
                    candidate[0] = '\n';
                    candidate[1] = '\0';
                }
                break;
            case 'I':
                if (candidate[0] == '\n')
                    candidate[1] = (char) c;
                break;
            case 'D':
                if (candidate[0] == '\n' && candidate[1] == 'I')
                    candidate[2] = (char) c;
                break;
            case '=':
                if (candidate[0] == '\n' && candidate[1] == 'I' && candidate[2] == 'D') {
                    for (i = 0; i < 4; i++)
                        candidate[i] = '\0';
                    i = 0;
                    correct_line = true;
                }
                break;
            default:
                if (correct_line)
                    candidate[i++] = (char) c;
        }
        if(break_loop) break;
    }

    if (ferror(fp))
        perror("I/O error when reading");

    fclose(fp);

    return correct_line ? candidate : "" /*"Unable to parse out distribution name"*/;
}

const char* dist_from_os_release(void) {
    const char* filenames[] = {"/usr/lib/os-release", "/etc/os-release"};
    size_t i;
    for(i=0; i<sizeof filenames / sizeof filenames[0]; i++) {
        const char* content = parse_id_from_os_release(filenames[i]);
        if (strlen(content) != 0)
            return content;
    }
    return "";
}
