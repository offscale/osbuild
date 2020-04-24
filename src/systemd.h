#ifndef OSBUILD_SYSTEMD_H
#define OSBUILD_SYSTEMD_H

#ifdef __linux__

const char* parse_id_from_os_release(const char*);

const char* dist_from_os_release(void);

#endif
#endif
