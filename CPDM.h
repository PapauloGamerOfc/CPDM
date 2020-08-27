#ifndef CPDM_H
#define CPDM_H

#ifdef _WIN32
#include <windows.h>
typedef HMODULE CPDM_t;
#else
#include <dlfcn.h>
typedef void *CPDM_t;
#endif

#ifdef __cplusplus
extern "C" {
#endif

CPDM_t CPDM_Open(const char *filename);

void *CPDM_Sym(CPDM_t handle, const char *symbol);

int CPDM_Close(CPDM_t handle);

#ifdef __cplusplus
}
#endif

#endif // CPDM_H