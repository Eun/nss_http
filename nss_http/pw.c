#include "../shared/global.h"
#include "../shared/log.h"
#include "../shared/user_object.h"
#include "../shared/user.h"

#include <stdlib.h>
#include <string.h>
#include <nss.h>
#include <pwd.h>


enum nss_status _nss_http_setpwent(void)
{
    log(LOG_INFO, "_nss_http_setpwent\n");
    return NSS_STATUS_SUCCESS;
}
enum nss_status _nss_http_endpwent(void)
{
    log(LOG_INFO, "_nss_http_endpwent\n");
    return NSS_STATUS_SUCCESS;
}

enum nss_status _nss_http_getpwent_r(struct passwd* resultbuf, char* buffer, size_t __buflen, struct passwd **result)
{
    log(LOG_INFO, "_nss_http_getpwent_r\n");
    return NSS_STATUS_NOTFOUND;
}

enum nss_status _nss_http_getpwnam_r(const char* name, struct passwd* result, char* buffer, size_t buflen, int* errnop)
{

    struct user_object user;
    enum nss_status status = get_user(name, 0, &user, buffer, buflen);

    if (status == NSS_STATUS_SUCCESS) {
        result->pw_name = user.User;
        result->pw_passwd = user.ReserverdPassword;
        result->pw_gecos = user.Name;
        result->pw_dir = user.Dir;
        result->pw_shell = user.Shell;
        result->pw_uid = user.Uid;
        result->pw_gid = user.Gid;
        *errnop = 0;

        clear_user(&user);
    }
    return status;
}


enum nss_status _nss_http_getpwuid_r(unsigned int uid, struct passwd* result, char* buffer, size_t buflen, int* errnop)
{
    struct user_object user;
    enum nss_status status = get_user(NULL, uid, &user, buffer, buflen);

    if (status == NSS_STATUS_SUCCESS) {
        result->pw_name = user.User;
        result->pw_passwd = user.ReserverdPassword;
        result->pw_gecos = user.Name;
        result->pw_dir = user.Dir;
        result->pw_shell = user.Shell;
        result->pw_uid = user.Uid;
        result->pw_gid = user.Gid;
        *errnop = 0;

        clear_user(&user);
    }
    return status;
}
