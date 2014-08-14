/*
 *  logger.c
 *
 *  Copyright (c) 2014 Daniel Rocha. All rights reserved.
 */

/* -------------------------------------------------------------------------- *
 *                               INCLUDED FILES                               *
 * -------------------------------------------------------------------------- */

#include <stdio.h>   /* fprintf()    */
#include <stdlib.h>  /* exit()       */
#include <time.h>    /* time(), ...  */
#include <stdarg.h>  /* va_list, ... */
#include "logger.h"

/* -------------------------------------------------------------------------- *
 *                              GLOBAL VARIABLES                              *
 * -------------------------------------------------------------------------- */

static struct {
    int level;
    int format;
    FILE *stream;
} logger = {LOG_LEVEL_ERROR, LOG_PRINT_ALL, NULL};

/* -------------------------------------------------------------------------- *
 *                                 PROTOTYPES                                 *
 * -------------------------------------------------------------------------- */

static const char *log_tag(int level);

/* -------------------------------------------------------------------------- *
 *                                 FUNCTIONS                                  *
 * -------------------------------------------------------------------------- */

/**
 * Sets the output stream where the logs are written.
 *
 * @param stream - File pointer to the logging stream
 */
void log_set_stream(FILE *stream)
{
    logger.stream = stream;
}

/**
 * Gets the current output stream.
 *
 * @return Current output stream file pointer.
 */
FILE *log_get_stream(void)
{
    return logger.stream;
}

/**
 * Sets the logging format.
 *
 * @param format - Logging format flags
 */
void log_set_format(int format)
{
    logger.format = format;
}

/**
 * Gets the current logging format.
 *
 * @return Current logging format flags.
 */
int log_get_format(void)
{
    return logger.format;
}

/**
 * Sets the logging severity level.
 *
 * @param level - Logging severity level
 */
void log_set_level(int level)
{
    logger.level = level;
}

/**
 * Gets the current logging severity level.
 *
 * @return Current logging severity level.
 */
int log_get_level(void)
{
    return logger.level;
}

/**
 * If the severity level of the call is higher than the logger's level, prints
 * the log message to the output stream. If an output stream was not defined,
 * "stderr" will be set and used instead. Logging a fatal message terminates
 * the program.
 *
 * @param file  - File name
 * @param line  - Line number
 * @param level - Severity level of the call
 * @param ...   - Message to be logged and its arguments
 */
void log_print(const char *file, int line, int level, ...)
{
    struct tm now;
    time_t secs;
    va_list msg;

    if (level < logger.level)
        return;

    if (logger.stream == NULL)
        logger.stream = stderr;

    time(&secs);
#ifdef WINDOWS
    localtime_s(&now, &secs);
#else
    localtime_r(&secs, &now);
#endif

    if (logger.format & LOG_PRINT_DATE)
        fprintf(logger.stream, "%04d-%02d-%02d - ",
                now.tm_year + 1900, now.tm_mon + 1, now.tm_mday);

    if (logger.format & LOG_PRINT_TIME)
        fprintf(logger.stream, "%02d:%02d:%02d - ",
                now.tm_hour, now.tm_min, now.tm_sec);

    if (logger.format & LOG_PRINT_FILE)
        fprintf(logger.stream, "%s:%d - ", file, line);

    if (logger.format & LOG_PRINT_TAG)
        fprintf(logger.stream, "%s - ", log_tag(level));

    va_start(msg, level);
    vfprintf(logger.stream, va_arg(msg, const char*), msg);
    va_end(msg);

    fprintf(logger.stream, "\n");

    if (level == LOG_LEVEL_FATAL) {
        fclose(logger.stream);
        exit(EXIT_FAILURE);
    }
}

/**
 * Returns the tag associated to a given severity level.
 *
 * @param level - Severity level of the call
 * @return A tag string associated to the input severity level.
 */
static const char *log_tag(int level)
{
    switch (level) {
    case LOG_LEVEL_TRACE:
        return "TRACE";
    case LOG_LEVEL_DEBUG:
        return "DEBUG";
    case LOG_LEVEL_INFO:
        return "INFO ";
    case LOG_LEVEL_WARN:
        return "WARN ";
    case LOG_LEVEL_ERROR:
        return "ERROR";
    case LOG_LEVEL_FATAL:
        return "FATAL";
    default:
        return "?????";
    }
}
