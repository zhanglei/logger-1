/*
 *  example.c
 *
 *  Copyright (c) 2014 Daniel Rocha. All rights reserved.
 */

#include <stdio.h>
#include "logger.h"

int main(void)
{
    FILE *logs[4];

    logs[0] = fopen("logs.txt", "w");
    logs[1] = fopen("logs_copy.txt", "w");
    logs[2] = stderr;
    logs[3] = NULL;

    log_set_streams(logs);
    log_set_level(LOG_LEVEL_DEBUG);

    log_debug("Starting.");
    log_trace("Passed here!");
    log_error("Port %d already in use.", 8080);
    log_fatal("Cannot start server, aborting.");
    log_debug("This should not show.");

    return 0;
}
