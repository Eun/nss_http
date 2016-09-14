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
    char buffer[1024];
    int error;
    assert(_nss_http_getpwnam_r("unknown_user_123456789", &result, (char*)&buffer, sizeof(buffer), &error) == NSS_STATUS_NOTFOUND);
    return 0;
}
