#include <syslog.h>
#define log(LOGLEVEL, ...) \
do \
{ \
    syslog(LOGLEVEL, APP_NAME": "__VA_ARGS__); \
} \
while (0)

#define loge(...) log(LOG_ERR, __VA_ARGS__)
#if LOG_LEVEL >= LOG_WARNING
	#define logw(...) log(LOG_WARNING, __VA_ARGS__)
#else
	#define logw(...) ((void)0)
#endif
#if LOG_LEVEL >= LOG_NOTICE
	#define logn(...) log(LOG_NOTICE, __VA_ARGS__)
#else
	#define logn(...) ((void)0)
#endif
#if LOG_LEVEL >= LOG_INFO
	#define logi(...) log(LOG_INFO, __VA_ARGS__)
#else
	#define logi(...) ((void)0)
#endif
#if LOG_LEVEL >= LOG_DEBUG
	#define logd(...) log(LOG_DEBUG, __VA_ARGS__)
#else
	#define logd(...) ((void)0)
#endif
