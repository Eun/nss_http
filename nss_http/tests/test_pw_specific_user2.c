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
    assert(_nss_http_getpwnam_r("joe", &result, (char*)&buffer, sizeof(buffer), &error) == NSS_STATUS_SUCCESS);

    assert(strcmp(result.pw_name, "joe") == 0);
    assert(strcmp(result.pw_gecos, "John Doe") == 0);
    assert(strcmp(result.pw_passwd, "x") == 0);
    assert(strcmp(result.pw_dir, "/home/joe") == 0);
    assert(strcmp(result.pw_shell, "/bin/bash") == 0);
    assert(result.pw_uid == 7000);
    assert(result.pw_gid == 7000);
    return 0;
}