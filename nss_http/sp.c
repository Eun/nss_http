#include "../shared/global.h"
#include "../shared/log.h"
#include "../shared/user_object.h"
#include "../shared/cache.h"
#include "../shared/comm.h"
#include "../shared/user.h"

#include <stdlib.h>
#include <string.h>
#include <nss.h>
#include <shadow.h>

enum nss_status _nss_http_setspent(void)
{
	log(LOG_INFO, "_nss_http_setspent\n");
	return NSS_STATUS_SUCCESS;
}
enum nss_status _nss_http_endspent(void)
{
	log(LOG_INFO, "_nss_http_endpwent\n");
	return NSS_STATUS_SUCCESS;
}

enum nss_status _nss_http_getspent_r(struct spwd* resultbuf, char* buffer, size_t __buflen, struct spwd **result)
{
	log(LOG_INFO, "_nss_http_getspwent_r\n");
	return NSS_STATUS_NOTFOUND;
}

enum nss_status _nss_http_getspnam_r(const char *name, struct spwd *result, char *buffer, size_t buflen, int *errnop)
{
	struct user_object user;
    enum nss_status status = get_user(name, 0, &user, buffer, buflen);

    if (status == NSS_STATUS_SUCCESS) {
        result->sp_namp = user.User;
        result->sp_pwdp = user.Password;
		result->sp_lstchg = -1;
		result->sp_min = -1;
		result->sp_max = -1;
		result->sp_warn = -1;
		result->sp_inact = -1;
		result->sp_expire = -1;
		result->sp_flag = -1;
        *errnop = 0;

        clear_user(&user);
    }
    return status;
}
