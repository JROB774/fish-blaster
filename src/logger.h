#ifndef LOGGER_H
#define LOGGER_H

typedef enum LogLevel__ { LOG_LEVEL_DEBUG, LOG_LEVEL_WARNING, LOG_LEVEL_ERROR } LogLevel;

#define LOGDEBUG(  ...) log_message(__FILE__, __LINE__, LOG_LEVEL_DEBUG,   __VA_ARGS__)
#define LOGWARNING(...) log_message(__FILE__, __LINE__, LOG_LEVEL_WARNING, __VA_ARGS__)
#define LOGERROR(  ...) log_message(__FILE__, __LINE__, LOG_LEVEL_ERROR,   __VA_ARGS__)

INTERNAL void quit_logger ();

#endif /* LOGGER_H */
