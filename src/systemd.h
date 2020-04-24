#ifndef OSBUILD_SYSTEMD_H
#define OSBUILD_SYSTEMD_H

#ifdef __linux__

extern const char* parse_id_from_os_release(const char*);

extern const char* dist_from_os_release(void);

#endif
#endif
