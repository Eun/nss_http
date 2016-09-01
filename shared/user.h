#include "global.h"
#include "user_object.h"


#include <stdlib.h>
#include <string.h>
#include <nss.h>

enum nss_status get_user(const char* name, unsigned int uid, struct user_object* user, char* buffer, size_t buflen);
char* sanitize_user_name(char* name);