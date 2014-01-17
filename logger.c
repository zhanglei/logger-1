/*
 *  logger.c
 *
 *  Copyright (c) 2014 Daniel Rocha. All rights reserved.
 */

/* ------------------------------------------------------------------------- *
 *                               INCLUDED FILES                              *
 * ------------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "logger.h"

/* ------------------------------------------------------------------------- *
 *                                 CONSTANTS                                 *
 * ------------------------------------------------------------------------- */
#define LOG_TAG_TRACE "TRACE"
#define LOG_TAG_DEBUG "DEBUG"
#define LOG_TAG_INFO  "INFO "
#define LOG_TAG_WARN  "WARN "
#define LOG_TAG_ERROR "ERROR"
#define LOG_TAG_FATAL "FATAL"
#define LOG_TAG_NULL  "?????"

/* ------------------------------------------------------------------------- *
 *                              GLOBAL VARIABLES                             *
 * ------------------------------------------------------------------------- */
static struct {
    int level;
    int format;
    FILE *stream;
} logger = {LOG_LEVEL_ERROR, LOG_PRINT_ALL, NULL};

/* ------------------------------------------------------------------------- *
 *                                 FUNCTIONS                                 *
 * ------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------- *
 * Function    : log_set_stream                                              *
 *                                                                           *
 * Description : Sets the default stream to output the logs.                 *
 * Input       : stream - File pointer                                       *
 * ------------------------------------------------------------------------- */
void log_set_stream(FILE *stream)
{
    logger.stream = stream;
}

/* ------------------------------------------------------------------------- *
 * Function    : log_get_stream                                              *
 *                                                                           *
 * Description : Gets the current output stream.                             *
 * Output      : Current output stream file pointer                          *
 * ------------------------------------------------------------------------- */
FILE *log_get_stream()
{
    return logger.stream;
}

/* ------------------------------------------------------------------------- *
 * Function    : log_set_format                                              *
 *                                                                           *
 * Description : Sets the logging format.                                    *
 * Input       : format - Logging format flags                               *
 * ------------------------------------------------------------------------- */
void log_set_format(int format)
{
    logger.format = format;
}

/* ------------------------------------------------------------------------- *
 * Function    : log_get_format                                              *
 *                                                                           *
 * Description : Gets the current logging format.                            *
 * Output      : Current logging format flags                                *
 * ------------------------------------------------------------------------- */
int log_get_format()
{
    return logger.format;
}

/* ------------------------------------------------------------------------- *
 * Function    : log_set_level                                               *
 *                                                                           *
 * Description : Sets the logging severity level.                            *
 * Input       : level - Logging severity level                              *
 * ------------------------------------------------------------------------- */
void log_set_level(int level)
{
    logger.level = level;
}

/* ------------------------------------------------------------------------- *
 * Function    : log_get_level                                               *
 *                                                                           *
 * Description : Gets the current logging severity level.                    *
 * Output      : Current logging severity level                              *
 * ------------------------------------------------------------------------- */
int log_get_level()
{
    return logger.level;
}

/* ------------------------------------------------------------------------- *
 * Function    : log_print                                                   *
 *                                                                           *
 * Description : If the severity level of the call is higher than the        *
 *               logger's level, prints the log message to the output        *
 *               stream. If an output stream was not defined, "stderr"       *
 *               will be set and used instead. Logging a fatal message       *
 *               terminates the program.                                     *
 * Input       : file  - File name                                           *
 *               line  - Line number                                         *
 *               level - Severity level of the call                          *
 *               msg   - Message to be logged                                *
 * ------------------------------------------------------------------------- */
void log_print(const char *file, int line, int level, const char *msg)
{
    const char *tag;
    struct tm *now;
    time_t tmp;

    if (level < logger.level)
        return;

    if (logger.stream == NULL)
        logger.stream = stderr;

    switch (level) {
    case LOG_LEVEL_TRACE:
        tag = LOG_TAG_TRACE;
        break;
    case LOG_LEVEL_DEBUG:
        tag = LOG_TAG_DEBUG;
        break;
    case LOG_LEVEL_INFO:
        tag = LOG_TAG_INFO;
        break;
    case LOG_LEVEL_WARN:
        tag = LOG_TAG_WARN;
        break;
    case LOG_LEVEL_ERROR:
        tag = LOG_TAG_ERROR;
        break;
    case LOG_LEVEL_FATAL:
        tag = LOG_TAG_FATAL;
        break;
    default:
        tag = LOG_TAG_NULL;
        break;
    }

    time(&tmp);
    now = localtime(&tmp);
    now->tm_year += 1900;
    now->tm_mon += 1;

    fprintf(logger.stream, "--");

    if (logger.format & LOG_PRINT_DATE)
        fprintf(logger.stream, "- %4d-%02d-%02d ",
                now->tm_year, now->tm_mon, now->tm_mday);

    if (logger.format & LOG_PRINT_TIME)
        fprintf(logger.stream, "- %02d:%02d:%02d ",
                now->tm_hour, now->tm_min, now->tm_sec);

    if (logger.format & LOG_PRINT_FILE)
        fprintf(logger.stream, "- %s:%d ", file, line);

    if (logger.format & LOG_PRINT_TAG)
        fprintf(logger.stream, "- %s ", tag);

    if (logger.format & LOG_PRINT_MSG)
        fprintf(logger.stream, "- %s ", msg);

    fprintf(logger.stream, "\n");
    fflush(logger.stream);

    if (level == LOG_LEVEL_FATAL)
        exit(EXIT_FAILURE);
}