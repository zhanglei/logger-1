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
#include <pthread.h>
#include "logger.h"

/* -------------------------------------------------------------------------- *
 *                              GLOBAL VARIABLES                              *
 * -------------------------------------------------------------------------- */

static struct {
    int format;
    int level;
    pthread_rwlock_t lock;
    FILE **streams;
} logger = {
    LOG_PRINT_DATE | LOG_PRINT_TIME | LOG_PRINT_TAG,
    LOG_LEVEL_INFO,
    PTHREAD_RWLOCK_INITIALIZER,
    NULL
};

/* -------------------------------------------------------------------------- *
 *                                 PROTOTYPES                                 *
 * -------------------------------------------------------------------------- */

static const char *log_tag(int level);

/* -------------------------------------------------------------------------- *
 *                                 FUNCTIONS                                  *
 * -------------------------------------------------------------------------- */

/**
 * Sets the output streams where the logs are written. A NULL terminated array
 * is passed as parameter containing a list of FILE pointers.
 *
 * @param streams - Array of file pointers
 */
void log_set_streams(FILE **streams)
{
    pthread_rwlock_wrlock(&logger.lock);
    logger.streams = streams;
    pthread_rwlock_unlock(&logger.lock);
}

/**
 * Gets the current array of output streams. The result of this function is NOT
 * thread-safe.
 *
 * @return Current array of output streams.
 */
FILE **log_get_streams(void)
{
    FILE **streams;

    pthread_rwlock_rdlock(&logger.lock);
    streams = logger.streams;
    pthread_rwlock_unlock(&logger.lock);

    return streams;
}

/**
 * Sets the logging format.
 *
 * @param format - Logging format flags
 */
void log_set_format(int format)
{
    pthread_rwlock_wrlock(&logger.lock);
    logger.format = format;
    pthread_rwlock_unlock(&logger.lock);
}

/**
 * Gets the current logging format.
 *
 * @return Current logging format flags.
 */
int log_get_format(void)
{
    int format;

    pthread_rwlock_rdlock(&logger.lock);
    format = logger.format;
    pthread_rwlock_unlock(&logger.lock);

    return format;
}

/**
 * Sets the logging severity level.
 *
 * @param level - Logging severity level
 */
void log_set_level(int level)
{
    pthread_rwlock_wrlock(&logger.lock);
    logger.level = level;
    pthread_rwlock_unlock(&logger.lock);
}

/**
 * Gets the current logging severity level.
 *
 * @return Current logging severity level.
 */
int log_get_level(void)
{
    int level;

    pthread_rwlock_rdlock(&logger.lock);
    level = logger.level;
    pthread_rwlock_unlock(&logger.lock);

    return level;
}

/**
 * If the severity level of the call is higher than the logger's level, prints
 * the log message to the output stream. If an output stream was not defined,
 * "stdout" will be used instead. Logging a fatal message terminates the
 * program.
 *
 * @param file  - File name
 * @param line  - Line number
 * @param level - Severity level of the call
 * @param ...   - Message to be logged and its arguments
 */
void log_print(const char *file, int line, int level, ...)
{
    FILE *DEFAULT_STREAMS[2] = {stdout, NULL};
    FILE **streams;
    time_t secs;
    struct tm now;
    va_list msg;
    unsigned int i;

    pthread_rwlock_rdlock(&logger.lock);

    if (level >= logger.level) {
        streams = logger.streams;
        if (streams == NULL)
            streams = DEFAULT_STREAMS;

        time(&secs);
        localtime_r(&secs, &now);
        now.tm_mon  += 1;
        now.tm_year += 1900;

        for (i = 0; streams[i] != NULL; ++i) {
            if (logger.format & LOG_PRINT_DATE)
                fprintf(streams[i], "%04d-%02d-%02d - ",
                        now.tm_year, now.tm_mon, now.tm_mday);

            if (logger.format & LOG_PRINT_TIME)
                fprintf(streams[i], "%02d:%02d:%02d - ",
                        now.tm_hour, now.tm_min, now.tm_sec);

            if (logger.format & LOG_PRINT_FILE)
                fprintf(streams[i], "%s:%d - ", file, line);

            if (logger.format & LOG_PRINT_TAG)
                fprintf(streams[i], "%s - ", log_tag(level));

            va_start(msg, level);
            vfprintf(streams[i], va_arg(msg, const char*), msg);
            va_end(msg);

            fprintf(streams[i], "\n");

            if (level == LOG_LEVEL_FATAL)
                fclose(streams[i]);
            else
                fflush(streams[i]);
        }
    }

    pthread_rwlock_unlock(&logger.lock);

    if (level == LOG_LEVEL_FATAL)
        exit(EXIT_FAILURE);
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
