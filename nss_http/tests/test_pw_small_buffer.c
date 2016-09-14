#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <error.h>
#include <nss.h>
#include <pwd.h>
#include <assert.h>


enum nss_status _nss_http_getpwnam_r(const char* name, struct passwd* result, char* buffer, size_t buflen, int* errnop);

int main()
{
    struct passwd result;
    char buffer[10];
    int error;
    assert(_nss_http_getpwnam_r("joe", &result, (char*)&buffer, sizeof(buffer), &error) == NSS_STATUS_TRYAGAIN);
    return 0;
}
