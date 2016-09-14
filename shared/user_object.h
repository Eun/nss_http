#pragma once
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <json-c/json.h>

struct user_object
{
    char *User;
    char *Name;
    char *Password;
    char *ReserverdPassword;
    char *Shell;
    char *Dir;
    int32_t Uid;
    int32_t Gid;
    int32_t AuthKeysSize;
    char **AuthKeys;
    struct json_object* CustomData;
};

enum nss_status user_object_from_json(const char* json_buffer, struct user_object* user, char* buffer, size_t buflen);
struct json_object* user_object_to_json(struct user_object* user);
void clear_user(struct user_object* user);