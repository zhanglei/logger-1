/*
 *  logger.h
 *
 *  Copyright (c) 2014 Daniel Rocha. All rights reserved.
 */

#ifndef LOGGER_H
#define LOGGER_H

/* -------------------------------------------------------------------------- *
 *                                 CONSTANTS                                  *
 * -------------------------------------------------------------------------- */

#define LOG_LEVEL_ALL   0
#define LOG_LEVEL_TRACE 1
#define LOG_LEVEL_DEBUG 2
#define LOG_LEVEL_INFO  3
#define LOG_LEVEL_WARN  4
#define LOG_LEVEL_ERROR 5
#define LOG_LEVEL_FATAL 6
#define LOG_LEVEL_OFF   7

#define LOG_PRINT_DATE  0x01
#define LOG_PRINT_TIME  0x02
#define LOG_PRINT_FILE  0x04
#define LOG_PRINT_TAG   0x08
#define LOG_PRINT_MSG   0x10
#define LOG_PRINT_ALL   0x1f

/* -------------------------------------------------------------------------- *
 *                                   MACROS                                   *
 * -------------------------------------------------------------------------- */

#define log_trace(...) \
        log_print(__FILE__, __LINE__, LOG_LEVEL_TRACE, __VA_ARGS__)

#define log_debug(...) \
        log_print(__FILE__, __LINE__, LOG_LEVEL_DEBUG, __VA_ARGS__)

#define log_info(...)  \
        log_print(__FILE__, __LINE__, LOG_LEVEL_INFO , __VA_ARGS__)

#define log_warn(...)  \
        log_print(__FILE__, __LINE__, LOG_LEVEL_WARN , __VA_ARGS__)

#define log_error(...) \
        log_print(__FILE__, __LINE__, LOG_LEVEL_ERROR, __VA_ARGS__)

#define log_fatal(...) \
        log_print(__FILE__, __LINE__, LOG_LEVEL_FATAL, __VA_ARGS__)

/* -------------------------------------------------------------------------- *
 *                                 PROTOTYPES                                 *
 * -------------------------------------------------------------------------- */

void  log_set_stream(FILE *stream);
FILE *log_get_stream(void);

void  log_set_format(int format);
int   log_get_format(void);

void  log_set_level(int level);
int   log_get_level(void);

void  log_print(const char *file, int line, int level, ...);

#endif
