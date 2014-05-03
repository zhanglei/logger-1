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
 * log_set_stream - Sets the default stream to output the logs.
 *
 * Parameters:
 *     stream : File pointer
 *
 * Returns: Nothing.
 */
void log_set_stream(FILE *stream)
{
    logger.stream = stream;
}

/**
 * log_get_stream - Gets the current output stream.
 *
 * Returns: Current output stream file pointer.
 */
FILE *log_get_stream(void)
{
    return logger.stream;
}

/**
 * log_set_format - Sets the logging format.
 *
 * Parameters:
 *     format : Logging format flags
 *
 * Returns: Nothing.
 */
void log_set_format(int format)
{
    logger.format = format;
}

/**
 * log_get_format - Gets the current logging format.
 *
 * Returns: Current logging format flags.
 */
int log_get_format(void)
{
    return logger.format;
}

/**
 * log_set_level - Sets the logging severity level.
 *
 * Parameters:
 *     level : Logging severity level
 *
 * Returns: Nothing.
 */
void log_set_level(int level)
{
    logger.level = level;
}

/**
 * log_get_level - Gets the current logging severity level.
 *
 * Returns: Current logging severity level.
 */
int log_get_level(void)
{
    return logger.level;
}

/**
 * log_print - If the severity level of the call is higher than the logger's
 *     level, prints the log message to the output stream. If an output stream
 *     was not defined, "stderr" will be set and used instead. Logging a fatal
 *     message terminates the program.
 *
 * Parameters:
 *     file  : File name
 *     line  : Line number
 *     level : Severity level of the call
 *     ...   : Message to be logged and its arguments
 *
 * Returns: Nothing.
 */
void log_print(const char *file, int line, int level, ...)
{
    struct tm *now;
    time_t tmp;
    va_list msgs;

    if (level < logger.level)
        return;

    if (logger.stream == NULL)
        logger.stream = stderr;

    time(&tmp);
    now = localtime(&tmp);
    now->tm_year += 1900;
    now->tm_mon += 1;

    fprintf(logger.stream, "-");

    if (logger.format & LOG_PRINT_DATE)
        fprintf(logger.stream, "- %4d-%02d-%02d ",
                now->tm_year, now->tm_mon, now->tm_mday);

    if (logger.format & LOG_PRINT_TIME)
        fprintf(logger.stream, "- %02d:%02d:%02d ",
                now->tm_hour, now->tm_min, now->tm_sec);

    if (logger.format & LOG_PRINT_FILE)
        fprintf(logger.stream, "- %s:%d ", file, line);

    if (logger.format & LOG_PRINT_TAG)
        fprintf(logger.stream, "- %s ", log_tag(level));

    if (logger.format & LOG_PRINT_MSG) {
        fprintf(logger.stream, "- ");

        va_start(msgs, level);
        vfprintf(logger.stream, va_arg(msgs, const char*), msgs);
        va_end(msgs);
    }

    fprintf(logger.stream, "\n");

    if (level == LOG_LEVEL_FATAL) {
        fclose(logger.stream);
        exit(EXIT_FAILURE);
    }
}

/**
 * log_tag - Returns the tag associated to a given severity level.
 *
 * Parameters:
 *     level : Severity level of the call
 *
 * Returns: A tag string associated to the input severity level.
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
