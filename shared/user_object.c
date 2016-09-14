#include "global.h"
#include "log.h"
#include "user_object.h"
#include <nss.h>
#include <json-c/json.h>
enum nss_status user_object_from_json(const char* json_buffer, struct user_object* user, char* buffer, size_t buflen)
{
    struct json_tokener* tok = json_tokener_new();
    json_object *json = json_tokener_parse_ex(tok, json_buffer, strlen(json_buffer));
    
    if (json == NULL) {
        loge("ERROR: Failed to parse json string\n");
        json_tokener_free(tok);
        return NSS_STATUS_NOTFOUND;
    }
    
    logd("DEBUG: Parsed JSON: %s\n", json_object_to_json_string(json));
    
    
    // get the required fields
    json_object *jfield;

    char *ptr = buffer;

    // add the "x" password for encrypted
    user->ReserverdPassword = ptr;
    *ptr++ = 'x';
    *ptr++ = 0;

    // get the "required" fields first
    if (!json_object_object_get_ex(json, "Password", &jfield)) {
        loge("ERROR: JSON does not contain 'Password' field\n");
        json_object_put(json);
        json_tokener_free(tok);
        return NSS_STATUS_NOTFOUND;
    }
    else {
        const char *field = json_object_get_string(jfield);
        int len = strlen(field);
        if (ptr - buffer + len + 1 > buflen) {
            json_object_put(json);
            json_tokener_free(tok);
            return NSS_STATUS_TRYAGAIN;
        }
        user->Password = ptr;
        memcpy(ptr, field, len);
        ptr += len;
        *ptr++ = 0;
    }

    logi("INFO: Parsed Password = %s\n", user->Password);


    if (!json_object_object_get_ex(json, "User", &jfield)) {
        loge("ERROR: JSON does not contain 'User' field\n");
        json_object_put(json);
        json_tokener_free(tok);
        return NSS_STATUS_NOTFOUND;
    }
    else {
        const char *field = json_object_get_string(jfield);
        int len = strlen(field);
        if (ptr - buffer + len + 1 > buflen) {
            json_object_put(json);
            json_tokener_free(tok);
            return NSS_STATUS_TRYAGAIN;
        }
        user->User = ptr;
        memcpy(ptr, field, len);
        ptr += len;
        *ptr++ = 0;
    }
    logi("INFO: Parsed User = %s\n", user->User);

    if (!json_object_object_get_ex(json, "Name", &jfield)) {
        logi("INFO: JSON does not contain 'Name' field\n");
        // if we have no "Name" field use the data stored in "User"
        user->Name = user->User;
    }
    else {
        const char *field = json_object_get_string(jfield);
        int len = strlen(field);
        if (ptr - buffer + len + 1 > buflen) {
            json_object_put(json);
            json_tokener_free(tok);
            return NSS_STATUS_TRYAGAIN;
        }
        user->Name = ptr;
        memcpy(ptr, field, len);
        ptr += len;
        *ptr++ = 0;
    }
    logi("INFO: Parsed Name = %s\n", user->Name);

    if (!json_object_object_get_ex(json, "Shell", &jfield)) {
        logi("INFO: JSON does not contain 'Shell' field\n");
        // if we have no "Shell" field use the DEFAULT_SHELL
        int len = strlen(DEFAULT_SHELL);
        if (ptr - buffer + len + 1 > buflen) {
            json_object_put(json);
            json_tokener_free(tok);
            return NSS_STATUS_TRYAGAIN;
        }
        user->Shell = ptr;
        memcpy(ptr, DEFAULT_SHELL, len);
        ptr += len;
        *ptr++ = 0;
    }
    else {
        const char *field = json_object_get_string(jfield);
        int len = strlen(field);
        if (ptr - buffer + len + 1 > buflen) {
            json_object_put(json);
            json_tokener_free(tok);
            return NSS_STATUS_TRYAGAIN;
        }
        user->Shell = ptr;
        memcpy(ptr, field, len);
        ptr += len;
        *ptr++ = 0;
    }
    logi("INFO: Parsed Shell = %s\n", user->Shell);

    if (!json_object_object_get_ex(json, "Dir", &jfield)) {
        logi("INFO: JSON does not contain 'Dir' field\n");
        // if we have no "Dir" field use the DEFAULT_DIR
        int len = strlen(DEFAULT_DIR);
        if (ptr - buffer + len + 1 > buflen) {
            json_object_put(json);
            json_tokener_free(tok);
            return NSS_STATUS_TRYAGAIN;
        }
        user->Dir = ptr;
        memcpy(ptr, DEFAULT_DIR, len);
        ptr += len;
        *ptr++ = 0;
    }
    else {
        const char *field = json_object_get_string(jfield);
        int len = strlen(field);
        if (ptr - buffer + len + 1 > buflen) {
            json_object_put(json);
            json_tokener_free(tok);
            return NSS_STATUS_TRYAGAIN;
        }
        user->Dir = ptr;
        memcpy(ptr, field, len);
        ptr += len;
        *ptr++ = 0;
    }
    logi("INFO: Parsed Dir = %s\n", user->Dir);

    if (!json_object_object_get_ex(json, "Uid", &jfield)) {
        logi("INFO: JSON does not contain 'Uid' field\n");
        // if we have no "Uid" field use the DEFAULT_UID
        user->Uid = DEFAULT_UID;
    }
    else {
        user->Uid = json_object_get_int(jfield);
    }
    logi("INFO: Parsed Uid = %d\n", user->Uid);

    if (!json_object_object_get_ex(json, "Gid", &jfield)) {
        logi("INFO: JSON does not contain 'Gid' field\n");
        // if we have no "Gid" field use the DEFAULT_GID
        user->Gid = DEFAULT_GID;
    }
    else {
        user->Gid = json_object_get_int(jfield);
    }
    logi("INFO: Parsed Gid = %d\n", user->Gid);

    if (!json_object_object_get_ex(json, "AuthKeys", &jfield)) {
        logi("INFO: JSON does not contain 'AuthKeys' field\n");
        // if we have no "Gid" field use the DEFAULT_GID
        user->AuthKeysSize = 0;
    }
    else {
        void *array = json_object_get_array(jfield);
        user->AuthKeysSize = array_list_length(array);
        // Allocate AuthKeys, because it wont fit in the buffer and we only need it in our ssh-key-helper
        if (user->AuthKeysSize > 0) {
            user->AuthKeys = (char**)malloc(sizeof(char*)*user->AuthKeysSize);
            for (int i = 0; i < user->AuthKeysSize; i++) {
                jfield = array_list_get_idx(array, i);
                const char* key = json_object_get_string(jfield);
                int len = strlen(key);
                user->AuthKeys[i] = (char*)malloc(sizeof(char*)*(len+1));
                memcpy(user->AuthKeys[i], key, len);
                user->AuthKeys[i][len] = 0;
            }
        }
    }
    logi("INFO: Parsed AuthKeysSize = %d\n", user->AuthKeysSize);


    if (!json_object_object_get_ex(json, "CustomData", &(user->CustomData))) {
        logi("INFO: JSON does not contain 'CustomData' field\n");
        // if we have no "Gid" field use the DEFAULT_GID
        user->CustomData = NULL;
    }
    else
    {
        // clone the object
        struct json_tokener* data_tok = json_tokener_new();
        const char* data = json_object_to_json_string(user->CustomData);
        user->CustomData = json_tokener_parse_ex(data_tok, data, strlen(data));
        logd("DEBUG: CUSTOMDATA: %s\n", data);
        json_tokener_free(data_tok);
    }
    
    

    json_object_put(json);
    json_tokener_free(tok);

    return NSS_STATUS_SUCCESS;
}

struct json_object* user_object_to_json(struct user_object* user)
{
    struct json_object* json = json_object_new_object();
    json_object_object_add(json, "User", json_object_new_string(user->User));
    json_object_object_add(json, "Name", json_object_new_string(user->Name));   
    json_object_object_add(json, "Password", json_object_new_string(user->Password));   
    json_object_object_add(json, "Shell", json_object_new_string(user->Shell));   
    json_object_object_add(json, "Dir", json_object_new_string(user->Dir));   
    json_object_object_add(json, "Uid", json_object_new_int(user->Uid));
    json_object_object_add(json, "Gid", json_object_new_int(user->Gid));

    if (user->AuthKeysSize > 0) {
        struct json_object* keys = json_object_new_array();
        for (int i = 0; i < user->AuthKeysSize; i++) {
            json_object_array_add(keys, json_object_new_string(user->AuthKeys[i]));
        }
        json_object_object_add(json, "AuthKeys", keys);
    }

    if (user->CustomData != NULL) {
        struct json_tokener* data_tok = json_tokener_new();
        const char* data = json_object_to_json_string(user->CustomData);
        struct json_object* customData = json_tokener_parse_ex(data_tok, data, strlen(data));
        json_tokener_free(data_tok);

        json_object_object_add(json, "CustomData", customData);
    }

    return json;
}

void clear_user(struct user_object* user)
{
    // clear all AuthKeys
    if (user->AuthKeysSize > 0) {
        for (int i = 0; i < user->AuthKeysSize; ++i)
        {
            free(user->AuthKeys[i]);
        }
        free(user->AuthKeys);
        user->AuthKeysSize = 0;
    }

    // clear custom data if it was used
    if (user->CustomData != NULL) {
        json_object_put(user->CustomData);
        user->CustomData = NULL;
    }
}