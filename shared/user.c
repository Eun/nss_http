#include "log.h"
#include "user.h"
#include "cache.h"
#include "comm.h"

#include <stdio.h>
#include <string.h>

enum nss_status get_user(const char* name, unsigned int uid, struct user_object* user, char* buffer, size_t buflen)
{
    log(LOG_DEBUG, "get_user: %s/%d\n", name, uid);
    enum nss_status status;

    char sanitized_name[MAX_NAME_SIZE + 1];

    // lookup via Uid or Name
    if (name == NULL) {
        status = cache_getByUid(uid, user, buffer, buflen);
    }
    else {
        // sanitize name, before first use
        int len = strlen(name);
        if (len > MAX_NAME_SIZE) {
            // username exceeds MAX_NAME_SIZE
            return NSS_STATUS_NOTFOUND;
        }
        memcpy(sanitized_name, name, len);
        sanitized_name[len] = 0;

        if (strlen(sanitize_user_name(sanitized_name)) == 0) {
            // username is invalid
            return NSS_STATUS_NOTFOUND;
        }
        status = cache_getByName(sanitized_name, user, buffer, buflen);   
    }

    // cache has an item, but it has the missing state
    if (status == NSS_STATUS_CACHE_NOTFOUND) {
        return NSS_STATUS_NOTFOUND;
    }

    if (status == NSS_STATUS_NOTFOUND || status == NSS_STATUS_UNAVAIL) {
        if (name == NULL) {
            status = comm_getByUid(uid, user, buffer, buflen);
        }
        else {
            status = comm_getByName(sanitized_name, user, buffer, buflen);   
        }
        if (status == NSS_STATUS_SUCCESS) {
            if (!cache_add(user)) {
                log(LOG_INFO, "Could not add %s to cache\n", user->User);
            }
            return NSS_STATUS_SUCCESS;
        }
        // add to cache, even if we have not found, so we save some double lookups
        else if (status == NSS_STATUS_NOTFOUND) {
            if (name == NULL) {
                if (!cache_add_missing_uid(uid)) {
                    log(LOG_INFO, "Could not add %d to cache\n", uid);
                }
            }
            else {
                if (!cache_add_missing_name(sanitized_name)) {
                    log(LOG_INFO, "Could not add %s to cache\n", sanitized_name);
                }
            }
        }
    }
    return status;
}

// sanitizes name after
// ^[a-z][-_a-z0-9]*$
char* sanitize_user_name(char* name)
{
    char* ptr = name;

    if (*ptr)
    {
        // start with a letter
        if ((*ptr >= 'a' && *ptr <= 'z') ||
            (*ptr >= 'A' && *ptr <= 'Z'))
        {
            ptr++;
        }
        else {
            memmove(ptr, ptr+1, strlen(name) + 1 - (int)(ptr-name));
        }
        // all following 
        while (*ptr)
        {
            if ((*ptr >= 'a' && *ptr <= 'z') ||
                (*ptr >= 'A' && *ptr <= 'Z') ||
                (*ptr >= '0' && *ptr <= '9') ||
                (*ptr == '-') ||
                (*ptr == '_')) {
                ptr++;
            }
            else {
                memmove(ptr, ptr+1, strlen(name) + 1 - (int)(ptr-name));
            }
        }
    }
    return name;
}