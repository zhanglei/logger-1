/*
 *  example.c
 *
 *  Copyright (c) 2014 Daniel Rocha. All rights reserved.
 */

#include <stdio.h>
#include "logger.h"

int main()
{
    log_set_level(LOG_LEVEL_DEBUG);
    log_set_format(LOG_PRINT_TIME | LOG_PRINT_TAG | LOG_PRINT_MSG);

    log_debug("Starting.");
    log_trace("Passed here!");
    log_fatal("Cannot start server, aborting.");
    log_debug("This should not show.");

    return 0;
}
