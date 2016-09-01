#pragma once

#include <string.h>
#include <nss.h>
#include "user_object.h"
#include <stdbool.h>

// Custom Code for "Entry is in Cache, but it has the missing state"
#define NSS_STATUS_CACHE_NOTFOUND 255

enum nss_status cache_getByName(const char* name, struct user_object* user, char* buffer, size_t buflen);
enum nss_status cache_getByUid(unsigned int uid, struct user_object* user, char* buffer, size_t buflen);
bool cache_add(struct user_object* user);
bool cache_add_missing_name(const char* name);
bool cache_add_missing_uid(unsigned int uid);