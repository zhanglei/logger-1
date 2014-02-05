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
#include <stdarg.h>
#include "logger.h"

/* ------------------------------------------------------------------------- *
 *                              GLOBAL VARIABLES                             *
 * ------------------------------------------------------------------------- */
static struct {
    int level;
    int format;
    FILE *stream;
} logger = {LOG_LEVEL_ERROR, LOG_PRINT_ALL, NULL};

/* ------------------------------------------------------------------------- *
 *                                 PROTOTYPES                                *
 * ------------------------------------------------------------------------- */
static const char *log_tag(int level);

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
 * Output      : Current output stream file pointer.                         *
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
 * Output      : Current logging format flags.                               *
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
 * Output      : Current logging severity level.                             *
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
 *                                                                           *
 * Input       : file  - File name                                           *
 *               line  - Line number                                         *
 *               level - Severity level of the call                          *
 *               msg   - Message to be logged                                *
 * ------------------------------------------------------------------------- */
void log_print(const char *file, int line, int level, const char *msg, ...)
{
    struct tm *now;
    time_t tmp;
    va_list args;

    if (level < logger.level)
        return;

    if (logger.stream == NULL)
        logger.stream = stderr;

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
        fprintf(logger.stream, "- %s ", log_tag(level));

    if (logger.format & LOG_PRINT_MSG) {
        fprintf(logger.stream, "- ");

        va_start(args, msg);
        vfprintf(logger.stream, msg, args);
        va_end(args);

        fprintf(logger.stream, " ");
    }

    fprintf(logger.stream, "\n");

    if (level == LOG_LEVEL_FATAL) {
        fclose(logger.stream);
        exit(EXIT_FAILURE);
    }
}

/* ------------------------------------------------------------------------- *
 * Function    : log_tag                                                     *
 *                                                                           *
 * Description : Returns the tag associated to a given severity level.       *
 * Input       : level - Severity level of the call                          *
 * Output      : Tag string associated to the input severity level.          *
 * ------------------------------------------------------------------------- */
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
