// Backend settings
// ==============================================================================================
// URL of the backend, use https:// if possible
#define BACKEND_URL "http://localhost:8000/"

// use folliwing token for authentication
//   - adds an "X-Auth-Token" header with the value
//   - can be combined with BACKEND_AUTH_USER & BACKEND_AUTH_PASS
#define BACKEND_AUTH_TOKEN "Token1"

// use folliwing username for authentication
//   - BACKEND_AUTH_PASS must be set as well
//#define BACKEND_AUTH_USER ""

// use folliwing password for authentication
//   - BACKEND_AUTH_USER must be set as well
//#define BACKEND_AUTH_PASS ""

// Cache settings
// ==============================================================================================
// Which cache type to use?
#define CACHE CACHE_REDIS

// How long to store user data in cache
#define CACHE_TIMEOUT 60

// Timeout for data in cache that got the "missing" state:
//   - if our backend is not finding a user we tell the cache that the 
//     user is missing. This avoids multiple lookups for missing users in the backend
#define CACHE_MISSING_TIMEOUT 60


// Cache specific settings
// ==============================================================================================
// Redis Settings
// Hostname of redis
#define CACHE_REDIS_HOST "127.0.0.1"

// Port
#define	CACHE_REDIS_PORT 6379
// Connect timeout
#define	CACHE_REDIS_TIMEOUT { 1, 500000 } // 1.5 seconds

// Use following password for authentication
//#define	CACHE_REDIS_AUTH_PASSWORD "password"


// Default user settings
// ==============================================================================================
// If backend does not respond with a uid use:
#define DEFAULT_UID 10000
// If backend does not respond with a gid use:
#define DEFAULT_GID 10000
// If backend does not respond with a shell use:
#define DEFAULT_SHELL "/bin/false"
// If backend does not respond with a home dir use:
#define DEFAULT_DIR "/"

// Misc
// ==============================================================================================
// Maximum username size
#define MAX_NAME_SIZE 128

// Available cache types
#define CACHE_NONE 0
#define CACHE_REDIS 1

// Loglevel
//   possible values: LOG_ERR, LOG_WARNING, LOG_NOTICE, LOG_INFO, LOG_DEBUG
#define LOG_LEVEL LOG_DEBUG