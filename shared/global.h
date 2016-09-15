// Backend settings
// ==============================================================================================
// URL of the backend, use https:// if possible
#ifndef BACKEND_URL
    #define BACKEND_URL "http://localhost:8000/"
#endif

// use folliwing token for authentication
//   - adds an "X-Auth-Token" header with the value
//   - can be combined with BACKEND_AUTH_USER & BACKEND_AUTH_PASS
#ifndef BACKEND_AUTH_TOKEN
    #define BACKEND_AUTH_TOKEN "Token1"
#endif

// use folliwing username for authentication
//   - BACKEND_AUTH_PASS must be set as well
#ifndef BACKEND_AUTH_USER
    //#define BACKEND_AUTH_USER ""
#endif

// use folliwing password for authentication
//   - BACKEND_AUTH_USER must be set as well
#ifndef BACKEND_AUTH_PASS
    //#define BACKEND_AUTH_PASS ""
#endif

// Cache settings
// ==============================================================================================
// Which cache type to use?
#ifndef CACHE
    #define CACHE CACHE_REDIS
#endif

// How long to store user data in cache
#ifndef CACHE_TIMEOUT
    #define CACHE_TIMEOUT 60
#endif

// Timeout for data in cache that got the "missing" state:
//   - if our backend is not finding a user we tell the cache that the 
//     user is missing. This avoids multiple lookups for missing users in the backend
#ifndef CACHE_MISSING_TIMEOUT
    #define CACHE_MISSING_TIMEOUT 60
#endif


// Cache specific settings
// ==============================================================================================
// Redis Settings
// Hostname of redis
#ifndef CACHE_REDIS_HOST
    #define CACHE_REDIS_HOST "127.0.0.1"
#endif


// Port
#ifndef CACHE_REDIS_PORT
    #define CACHE_REDIS_PORT 6379
#endif

// Connect timeout
#ifndef CACHE_REDIS_TIMEOUT
    #define CACHE_REDIS_TIMEOUT { 1, 500000 } // 1.5 seconds
#endif


// Use following password for authentication
#ifndef CACHE_REDIS_AUTH_PASSWORD//
    //#define CACHE_REDIS_AUTH_PASSWORD "password"
#endif



// Default user settings
// ==============================================================================================
// If backend does not respond with a uid use:
#ifndef DEFAULT_UID
    #define DEFAULT_UID 10000
#endif

// If backend does not respond with a gid use:
#ifndef DEFAULT_GID
    #define DEFAULT_GID 10000
#endif

// If backend does not respond with a shell use:
#ifndef DEFAULT_SHELL
    #define DEFAULT_SHELL "/bin/false"
#endif

// If backend does not respond with a home dir use:
#ifndef DEFAULT_DIR
    #define DEFAULT_DIR "/"
#endif


// Misc
// ==============================================================================================
// Maximum username size
#ifndef MAX_NAME_SIZE
    #define MAX_NAME_SIZE 128
#endif


// Available cache types
#define CACHE_NONE 0
#define CACHE_REDIS 1

// Loglevel
//   possible values: LOG_ERR, LOG_WARNING, LOG_NOTICE, LOG_INFO, LOG_DEBUG
#ifndef LOG_LEVEL
    #define LOG_LEVEL LOG_WARNING
#endif
