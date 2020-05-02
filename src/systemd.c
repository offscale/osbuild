#ifdef __linux__

#include <stdio.h>
#include "stdbool.h"
#include <string.h>

#include "systemd.h"

/* What distribution is more than 80 characters?! */
#define MAX_DISTRIBUTION_LEN 80

const char* parse_id_from_os_release(const char* filename) {
    static char candidate[MAX_DISTRIBUTION_LEN];
    static char id[MAX_DISTRIBUTION_LEN];
    bool correct_line = false;
    bool id_like = false;
    bool set = false;
    bool end_loop = false;
    int c;
    size_t i, cursor;
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("File opening failed");
        return "";
    }

    candidate[0] = '\n';
    for (cursor = 0; (c = fgetc(fp)) != EOF;) {
        if (end_loop) break;

        switch (c) {
            case '"':
            case ' ':
            case '\n':
                if (correct_line) {
                    candidate[cursor+1] = '\0';
                    if (id_like) {
                        end_loop = true;
                        break;
                    }
                    for (i = 0; i < MAX_DISTRIBUTION_LEN; i++) {
                        id[i] = candidate[i];
                        if (candidate[i] == '\0')
                            break;
                    }

                    correct_line = false;
                }
                candidate[0] = (char)c;
                for (i = 1; i < 8; i++)
                    candidate[i] = '\0';

                cursor = 0;
                break;
            case '=':
                if (candidate[0] == 'I' && candidate[1] == 'D') {
                    if (candidate[2] == '_' && candidate[3] == 'L' /*&&
                        candidate[4] == 'I' && candidate[5] == 'K' &&
                        candidate[5] == 'K' && candidate[6] == 'E' &&
                        cursor == 7*/)
                        id_like = true;
                    /*if (cursor != 2 && cursor != 7)
                        printf("cursor unexpectedly %lu\n", cursor);*/

                    for (i = 0; i < 8; i++)
                        candidate[i] = '\0';
                    cursor = 0;
                    correct_line = true;
                }
                break;
            case 'I':
                if (!correct_line && candidate[0] == '\n' && (
                        cursor == 0 || (candidate[1] == 'D' && candidate[2] == '_' && cursor == 4)))
                    set = true;
            case 'D':
                if (candidate[0] == 'I' && cursor == 1)
                    set = true;
            case '_':
                if (candidate[0] == 'I' && candidate[1] == 'D' && cursor == 2)
                    set = true;
            case 'L':
                if (candidate[0] == 'I' && candidate[1] == 'D' && candidate[2] == '_' &&
                    cursor == 3)
                    set = true;
            case 'K':
                if (candidate[0] == 'I' && candidate[1] == 'D' && candidate[2] == '_' &&
                    candidate[3] == 'L' && candidate[4] == 'I' && cursor == 5)
                    set = true;
            case 'E':
                if (candidate[0] == 'I' && candidate[1] == 'D' && candidate[2] == '_' &&
                    candidate[3] == 'L' && candidate[4] == 'I' && candidate[5] == 'K' &&
                    cursor == 6)
                    set = true;
            default:
                if (correct_line || set)
                    candidate[cursor++] = (char) c;
        }
        set = false;
    }

    if (ferror(fp))
        perror("I/O error when reading");

    fclose(fp);

    /* printf("candidate: \"%s\";\nid: \"%s\"\n\n", candidate, id); */

    return correct_line ? (strlen(candidate) == 0 ? id : candidate) : NULL /*"Unable to parse out distribution name"*/;
}

const char* dist_from_os_release(void) {
    const char* filenames[2] = {"/usr/lib/os-release", "/etc/os-release"};
    size_t i;
    for(i=0; i<sizeof filenames / sizeof filenames[0]; i++) {
        const char* content = parse_id_from_os_release(filenames[i]);
        if (content != NULL)
            return content;
    }
    return NULL;
}

#endif
