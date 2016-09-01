#include "global.h"
#include "cache.h"
#include "log.h"

#if CACHE == CACHE_REDIS
    #include <stdbool.h>
    #include <hiredis/hiredis.h>
    
    struct redisContext* cache_connect()
    {
        struct timeval timeout = CACHE_REDIS_TIMEOUT;
        redisContext* context = redisConnectWithTimeout(CACHE_REDIS_HOST, CACHE_REDIS_PORT, timeout);
        if (context == NULL || context->err) {
            if (context) {
                loge("cache: redis: Connection error: %s\n", context->errstr);
                redisFree(context);
            } else {
                loge("cache: redis: Connection error: can't allocate redis context\n");
            }
            return NULL;
        }

        #if defined(CACHE_REDIS_AUTH_PASSWORD)
            logd("cache: redis: Auth using password");
            struct redisReply* reply;
            reply = redisCommand(context, "AUTH "CACHE_REDIS_AUTH_PASSWORD);
            if (reply == NULL) {
                loge("cache: redis: Connection error: invalid password\n");
                return NULL;
            }
            freeReplyObject(reply);
        #endif

        return context;
    }

    void cache_disconnect(redisContext* context)
    {
        redisFree(context);
    }
    enum nss_status cache_getByName(const char* name, struct user_object* user, char* buffer, size_t buflen)
    {
        struct redisContext* context = cache_connect();
        if (context == NULL)
        {
            return NSS_STATUS_UNAVAIL;
        }

        struct redisReply* reply = redisCommand(context, "GET %s", name);
        if (reply == NULL) {
            logi("cache: redis: User '%s' not in cache\n", name);
            cache_disconnect(context);
            return NSS_STATUS_NOTFOUND;
        }

        enum nss_status status = NSS_STATUS_NOTFOUND;
        logd("cache: redis: reply->type = %d\n",reply->type);
        if (reply->type == REDIS_REPLY_STRING) {
            if (strcmp(reply->str, "NULL") == 0) {
                freeReplyObject(reply);
                cache_disconnect(context);
                return NSS_STATUS_CACHE_NOTFOUND;
            }
            status = user_object_from_json(reply->str, user, buffer, buflen);
        }
        freeReplyObject(reply);
        cache_disconnect(context);
        return status;
    }

    enum nss_status cache_getByUid(unsigned int uid, struct user_object* user, char* buffer, size_t buflen)
    {
        struct redisContext* context = cache_connect();
        if (context == NULL)
        {
            return NSS_STATUS_UNAVAIL;
        }

        struct redisReply* reply = redisCommand(context, "GET UID-%d", uid);
        if (reply == NULL) {
            logi("cache: redis: Uid '%d' not in cache\n", uid);
            cache_disconnect(context);
            return NSS_STATUS_NOTFOUND;
        }
        enum nss_status status = NSS_STATUS_NOTFOUND;
        logd("cache: redis: reply->type = %d\n",reply->type);
        if (reply->type == REDIS_REPLY_STRING) {
            if (strcmp(reply->str, "NULL") == 0) {
                freeReplyObject(reply);
                cache_disconnect(context);
                return NSS_STATUS_CACHE_NOTFOUND;
            }
            status = cache_getByName(reply->str, user, buffer, buflen);
        }
        freeReplyObject(reply);
        cache_disconnect(context);
        return status;
    }

    bool cache_add(struct user_object* user)
    {
        struct redisContext* context = cache_connect();
        if (context == NULL)
        {
            return NSS_STATUS_UNAVAIL;
        }

        json_object* json = user_object_to_json(user);
        const char* buffer = json_object_to_json_string(json);
        logd("cache: redis: SET %s %s\n", user->User, buffer);
        struct redisReply* reply = redisCommand(context, "SET %s %s", user->User, buffer);
        json_object_put(json);
        if (reply == NULL) {
            cache_disconnect(context);
            return false;
        }
        if (reply->type == REDIS_REPLY_ERROR)
        {
            loge("cache: redis: Could not SET data: %s\n", reply->str);
            freeReplyObject(reply);
            cache_disconnect(context);
            return false;
        }
        freeReplyObject(reply);

        logd("cache: redis: SET UID-%d %s\n", user->Uid, user->User);
        reply = redisCommand(context, "SET UID-%d %s", user->Uid, user->User);
        if (reply == NULL) {
            cache_disconnect(context);
            return false;
        }
        if (reply->type == REDIS_REPLY_ERROR)
        {
            loge("cache: redis: Could not SET data: %s\n", reply->str);
            freeReplyObject(reply);
            cache_disconnect(context);
            return false;
        }
        freeReplyObject(reply);

        reply = redisCommand(context, "EXPIRE %s %d", user->User, CACHE_TIMEOUT);
        if (reply == NULL) {
            cache_disconnect(context);
            return false;
        }
        if (reply->type == REDIS_REPLY_ERROR)
        {
            loge("cache: redis: Could not SET TIMEOUT: %s\n", reply->str);
            freeReplyObject(reply);
            cache_disconnect(context);
            return false;
        }
        freeReplyObject(reply);
        cache_disconnect(context);
        return true;
    }
    bool cache_add_missing_name(const char* name)
    {
        struct redisContext* context = cache_connect();
        if (context == NULL)
        {
            return NSS_STATUS_UNAVAIL;
        }

        logd("cache: redis: SET %s NULL\n", name);
        struct redisReply* reply = redisCommand(context, "SET %s NULL", name);
        if (reply == NULL) {
            cache_disconnect(context);
            return false;
        }
        if (reply->type == REDIS_REPLY_ERROR)
        {
            loge("cache: redis: Could not SET data: %s\n", reply->str);
            freeReplyObject(reply);
            cache_disconnect(context);
            return false;
        }
        freeReplyObject(reply);

        reply = redisCommand(context, "EXPIRE %s %d", name, CACHE_MISSING_TIMEOUT);
        if (reply == NULL) {
            cache_disconnect(context);
            return false;
        }
        if (reply->type == REDIS_REPLY_ERROR)
        {
            loge("cache: redis: Could not SET TIMEOUT: %s\n", reply->str);
            freeReplyObject(reply);
            cache_disconnect(context);
            return false;
        }
        freeReplyObject(reply);
        cache_disconnect(context);
        return true;
    }
    bool cache_add_missing_uid(unsigned int uid)
    {
        struct redisContext* context = cache_connect();
        if (context == NULL)
        {
            return NSS_STATUS_UNAVAIL;
        }

        logd("cache: redis: SET UID-%d NULL\n", uid);
        struct redisReply* reply = redisCommand(context, "SET UID-%d NULL", uid);
        if (reply == NULL) {
            cache_disconnect(context);
            return false;
        }
        if (reply->type == REDIS_REPLY_ERROR)
        {
            loge("cache: redis: Could not SET data: %s\n", reply->str);
            freeReplyObject(reply);
            cache_disconnect(context);
            return false;
        }
        freeReplyObject(reply);

        reply = redisCommand(context, "EXPIRE UID-%d %d", uid, CACHE_MISSING_TIMEOUT);
        if (reply == NULL) {
            cache_disconnect(context);
            return false;
        }
        if (reply->type == REDIS_REPLY_ERROR)
        {
            loge("cache: redis: Could not SET TIMEOUT: %s\n", reply->str);
            freeReplyObject(reply);
            cache_disconnect(context);
            return false;
        }
        freeReplyObject(reply);
        cache_disconnect(context);
        return true;
    }
#else
    enum nss_status cache_getByName(const char* name, struct user_object* user, char* buffer, size_t buflen)
    {
        return NSS_STATUS_NOTFOUND;
    }
    enum nss_status cache_getByUid(unsigned int uid, struct user_object* user, char* buffer, size_t buflen)
    {
        return NSS_STATUS_NOTFOUND;
    }
    bool cache_add(struct user_object* user)
    {
        return false;
    }
    bool cache_add_missing_name(const char* name)
    {
        return false;
    }
    bool cache_add_missing_uid(unsigned int uid)
    {
        return false;
    }
#endif