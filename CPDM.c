#include "CPDM.h"
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>

CPDM_t CPDM_Open(const char *filename)
{
    CPDM_t handle = LoadLibrary(filename);
    void (*onload) () = NULL;
    if(handle)
    {
        onload = CPDM_Sym(handle, "onload");
        if(onload)
        {
            onload();
        }
    }
    return handle;
}

void *CPDM_Sym(CPDM_t handle, const char *symbol)
{
    return GetProcAddress(handle, symbol);
}

int CPDM_Close(CPDM_t handle)
{
    return FreeLibrary(handle);
}

#else
#include <dlfcn.h>
#include <string.h>
#include <stdint.h>

CPDM_t CPDM_Open(const char *filename)
{
    char *temp = NULL;
    CPDM_t handle = NULL;
    size_t len1, len2, sum;
    void (*onload) () = NULL;
    if(strncmp(filename, "./", len1 = strlen("./")))
    {
        len2 = strlen(filename);
        if((sum = len1 + len2) < len1 || sum < len2 || sum == SIZE_MAX)
            return NULL;
        temp = malloc(sum + 1);
        if(!temp) return NULL;
        memcpy(temp, "./", len1);
        memcpy(temp + len1, filename, len2);
        temp[sum] = '\0';
    }
    handle = dlopen(temp == NULL ? filename : temp, RTLD_LAZY | RTLD_LOCAL);
    if(temp) free(temp);
    if(handle)
    {
        onload = CPDM_Sym(handle, "onload");
        if(onload)
        {
            onload();
        }
    }
    return handle;
}

void *CPDM_Sym(CPDM_t handle, const char *symbol)
{
    return dlsym(handle, symbol);
}

int CPDM_Close(CPDM_t handle)
{
    return dlclose(handle);
}

#endif // has_include