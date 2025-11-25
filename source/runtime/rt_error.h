#pragma once

enum RtErrorCode
{
    RT_OK = 0,
    RT_ERR_IO = 1,
    RT_ERR_INVALID = 2
};

const char *rt_error_to_string(RtErrorCode code);
