/*
 *  logger.h
 *
 *  Copyright (c) 2014 Daniel Rocha. All rights reserved.
 */

#ifndef LOGGER_H
#define LOGGER_H

/* ------------------------------------------------------------------------- *
 *                                 CONSTANTS                                 *
 * ------------------------------------------------------------------------- */
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

/* ------------------------------------------------------------------------- *
 *                                   MACROS                                  *
 * ------------------------------------------------------------------------- */
#define LOG_TRACE(msg)  log_print(__FILE__, __LINE__, LOG_LEVEL_TRACE, (msg))
#define LOG_DEBUG(msg)  log_print(__FILE__, __LINE__, LOG_LEVEL_DEBUG, (msg))
#define LOG_INFO(msg)   log_print(__FILE__, __LINE__, LOG_LEVEL_INFO , (msg))
#define LOG_WARN(msg)   log_print(__FILE__, __LINE__, LOG_LEVEL_WARN , (msg))
#define LOG_ERROR(msg)  log_print(__FILE__, __LINE__, LOG_LEVEL_ERROR, (msg))
#define LOG_FATAL(msg)  log_print(__FILE__, __LINE__, LOG_LEVEL_FATAL, (msg))

/* ------------------------------------------------------------------------- *
 *                                 PROTOTYPES                                *
 * ------------------------------------------------------------------------- */
void  log_set_stream(FILE *stream);
FILE *log_get_stream();

void  log_set_format(int format);
int   log_get_format();

void  log_set_level(int level);
int   log_get_level();

void  log_print(const char *file, int line, int level, const char *msg);

#endif
