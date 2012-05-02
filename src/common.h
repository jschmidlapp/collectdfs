
#ifndef _COMMON_H_
#define _COMMOH_H_

#define foreach(i,d) for(i = d.begin(); i != d.end(); i++)
#define foreach_ptr(i,d) for(i = d->begin(); i != d->end(); i++)

#define LOG_LEVEL_ERROR 1
#define LOG_LEVEL_INFO  2
#define LOG_LEVEL_DEBUG 3

#define LOG(level, ...)  \
  if (level > m_log_level) ; \
  else printf("[ collectd-fs ]: "__VA_ARGS__);

#define DEBUG(...)  LOG(LOG_LEVEL_DEBUG, __VA_ARGS__)
#define ERROR(...)  LOG(LOG_LEVEL_ERROR, __VA_ARGS__)
#define INFO(...)   LOG(LOG_LEVEL_INFO, __VA_ARGS__)

#endif /* _COMMON_H_ */
