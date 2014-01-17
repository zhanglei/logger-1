#include <stdio.h>
#include "logger.h"

int main()
{
    log_set_level(LOG_LEVEL_DEBUG);
    log_set_format(LOG_PRINT_TIME | LOG_PRINT_TAG | LOG_PRINT_MSG);

    LOG_DEBUG("Starting.");
    LOG_TRACE("Passed here!");
    LOG_FATAL("Cannot start server, aborting.");
    LOG_DEBUG("This should not show.");

    return 0;
}
