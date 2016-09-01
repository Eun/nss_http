#pragma once

#include <nss.h>
#include "string.h"
#include "user_object.h"

#define OutputChunkSize 128
struct chunked_output
{
    char *ptr;
    int len;
    int size;
};


enum nss_status comm_getByName(const char* name, struct user_object* user, char* buffer, size_t buflen);
enum nss_status comm_getByUid(unsigned int uid, struct user_object* user, char* buffer, size_t buflen);
