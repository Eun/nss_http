#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <error.h>
#include <nss.h>
#include <shadow.h>
#include <assert.h>
#include <crypt.h>

enum nss_status _nss_http_getspnam_r(const char* name, struct spwd* result, char* buffer, size_t buflen, int* errnop);


void testGetSpecificUser()
{
    struct spwd result;
    char buffer[1024];
    int error;
    assert(_nss_http_getspnam_r("smith", &result, (char*)&buffer, sizeof(buffer), &error) == NSS_STATUS_SUCCESS);

    assert(strcmp(result.sp_namp, "smith") == 0);

    char salt[strlen(result.sp_pwdp)];
    memcpy(salt, result.sp_pwdp, strlen(result.sp_pwdp));

    char *ptr = strchr(salt, '$');
    assert(ptr != NULL);
    ptr = strchr(ptr+1, '$');
    assert(ptr != NULL);
    ptr = strchr(ptr+1, '$');
    assert(ptr != NULL);
    ptr[0] = 0;

    char* pass = crypt("matrix", salt);
    assert(strcmp(result.sp_pwdp, pass) == 0);

}


void testGetUnkownUser()
{
    struct spwd result;
    char buffer[1024];
    int error;
    assert(_nss_http_getspnam_r("unknown_user_123456789", &result, (char*)&buffer, sizeof(buffer), &error) == NSS_STATUS_NOTFOUND);
}

void testSmallBuffer()
{
    struct spwd result;
    char buffer[10];
    int error;
    assert(_nss_http_getspnam_r("joe", &result, (char*)&buffer, sizeof(buffer), &error) == NSS_STATUS_TRYAGAIN);
}

int main()
{
    testGetSpecificUser();
    testGetUnkownUser();
    testSmallBuffer();
    return 0;
}