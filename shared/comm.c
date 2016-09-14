#include "global.h"
#include "comm.h"
#include "log.h"
#include "string.h"
#include <curl/curl.h>
#include <json-c/json.h>




static size_t comm_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
    size_t realsize = size * nmemb;
    log(LOG_DEBUG , "Writing chunk: %s (%ld)\n", ptr, realsize);
    struct chunked_output* output = ((struct chunked_output*)userdata);
    while (output->size < output->len + realsize) {
        output->size = output->size + OutputChunkSize;
        logd("Extending OutputString to  %d bytes\n", output->size);
        output->ptr = (char*)realloc(output->ptr, output->size);
        if (output->ptr == NULL)
        {
            log(LOG_INFO, "Could not expand string to %d\n", output->size);
            exit(-1);
        }
    }
    memcpy(output->ptr + output->len, ptr, realsize);
    output->len += realsize;
    return realsize;
}

enum nss_status _comm_get(const char* name, const unsigned int uid, struct user_object* user, struct chunked_output* output)
{
    json_object *json;
    CURL *curl;
    logd("Performing HTTP request for %s/%d using %s\n", name, uid, BACKEND_URL);
    curl = curl_easy_init();

    if (!curl) {
        return NSS_STATUS_UNAVAIL;
    }

    #if defined(BACKEND_AUTH_USER) && defined(BACKEND_AUTH_PASS)
        logd("Enabling UserAuth\n");
        curl_easy_setopt(curl, CURLOPT_USERPWD, BACKEND_AUTH_USER":"BACKEND_AUTH_PASS);
    #endif


    curl_easy_setopt(curl, CURLOPT_URL, BACKEND_URL);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, output);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, comm_write_callback);
    curl_easy_setopt(curl, CURLOPT_HEADER, 0);
    
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1);
    curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 1);

    curl_easy_setopt(curl, CURLOPT_RANDOM_FILE, "/dev/urandom");
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2);
    curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);

    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
    curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 1);

    curl_easy_setopt(curl, CURLOPT_USERAGENT, "nss_http/1.0");
    curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1);

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Accept: application/json");
    headers = curl_slist_append(headers, "Content-Type: application/json");

    #if defined(BACKEND_AUTH_TOKEN)
        logd("Enabling Auth TOKEN\n");
        headers = curl_slist_append(headers, "X-Auth-Token: "BACKEND_AUTH_TOKEN);
    #endif

    // create json object for post
    json = json_object_new_object();

    // build post data
    if (name == NULL) {
        json_object_object_add(json, "Uid", json_object_new_int(uid));
    }
    else {
        json_object_object_add(json, "User", json_object_new_string(name));
    }

    // set curl options
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_object_to_json_string(json));


    logd("Performing Request...\n");
    int res = curl_easy_perform(curl);
    logd("Request Result: %d...\n", res);

    curl_slist_free_all(headers);

    json_object_put(json);

    if (res != CURLE_OK) {
        if (res != CURLE_HTTP_RETURNED_ERROR) {
            logw("WARNING: curl responded with an error: %d (user=%s)\n", res, name);
        }
        else {
            int httpCode;
            if (curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode) == CURLE_OK) {
                if (httpCode == 404) { 
                    curl_easy_cleanup(curl);
                    return NSS_STATUS_NOTFOUND;
                }
                else if (httpCode == 401) { 
                    loge("ERROR: Backend Server: authorization failed\n");
                }
                else {
                    logw("WARNING: Server responded with HTTP Code: %d (user=%s)\n", httpCode, name);
                }
            }   
        }
        curl_easy_cleanup(curl);
        return NSS_STATUS_UNAVAIL;
    } 
    curl_easy_cleanup(curl);
    output->ptr[output->len] = 0;
    return NSS_STATUS_SUCCESS;
}

enum nss_status comm_getByName(const char* name, struct user_object* user, char* buffer, size_t buflen)
{
    struct chunked_output output;
    output.len = 0;
    output.size = OutputChunkSize;
    output.ptr = (char*)malloc(OutputChunkSize);
    enum nss_status status = _comm_get(name, 0, user, &output);
    if (status == NSS_STATUS_SUCCESS) {
        status = user_object_from_json(output.ptr, user, buffer, buflen);
    }
    free(output.ptr);
    return status;
}

enum nss_status comm_getByUid(unsigned int uid, struct user_object* user, char* buffer, size_t buflen)
{
    struct chunked_output output;
    output.len = 0;
    output.size = OutputChunkSize;
    output.ptr = (char*)malloc(OutputChunkSize);
    enum nss_status status = _comm_get(NULL, uid, user, &output);
    if (status == NSS_STATUS_SUCCESS) {
        status = user_object_from_json(output.ptr, user, buffer, buflen);
    }
    free(output.ptr);
    return status;
}