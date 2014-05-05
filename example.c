/*
 *  example.c
 *
 *  Copyright (c) 2014 Daniel Rocha. All rights reserved.
 */

#include <stdio.h>
#include "logger.h"

int main(void)
{
    log_set_level(LOG_LEVEL_DEBUG);
    log_set_format(LOG_PRINT_TAG | LOG_PRINT_DATE | LOG_PRINT_TIME);

    log_debug("Starting.");
    log_trace("Passed here!");
    log_error("Port %d already in use.", 8080);
    log_fatal("Cannot start server, aborting.");
    log_debug("This should not show.");

    return 0;
}
