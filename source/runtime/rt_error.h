#ifndef RUNTIME_RT_ERROR_H
#define RUNTIME_RT_ERROR_H

enum RuntimeErrorCode
{
    RUNTIME_ERROR_UNKNOWN = 0,
    RUNTIME_ERROR_FATAL = 1
};

void report_fatal_error(const char *message);

#endif /* RUNTIME_RT_ERROR_H */
