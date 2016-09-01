#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <error.h>
#include <nss.h>
#include <pwd.h>
#include <assert.h>


enum nss_status _nss_http_getpwnam_r(const char* name, struct passwd* result, char* buffer, size_t buflen, int* errnop);


void testGetSpecificUser()
{
    struct passwd result;
    char buffer[1024];
    int error;
    assert(_nss_http_getpwnam_r("smith", &result, (char*)&buffer, sizeof(buffer), &error) == NSS_STATUS_SUCCESS);

    assert(strcmp(result.pw_name, "smith") == 0);
    assert(strcmp(result.pw_gecos, "Mr. Smith") == 0);
    assert(strcmp(result.pw_passwd, "x") == 0);
    assert(strcmp(result.pw_dir, "/home/matrix") == 0);
    assert(strcmp(result.pw_shell, "/bin/bash") == 0);
    assert(result.pw_uid == 7001);
    assert(result.pw_gid == 7001);
}


void testGetUnkownUser()
{
    struct passwd result;
    char buffer[1024];
    int error;
    assert(_nss_http_getpwnam_r("unknown_user_123456789", &result, (char*)&buffer, sizeof(buffer), &error) == NSS_STATUS_NOTFOUND);
}

void testSmallBuffer()
{
    struct passwd result;
    char buffer[10];
    int error;
    assert(_nss_http_getpwnam_r("joe", &result, (char*)&buffer, sizeof(buffer), &error) == NSS_STATUS_TRYAGAIN);
}

int main()
{
    testGetSpecificUser();
    testGetUnkownUser();
    testSmallBuffer();
    return 0;
}