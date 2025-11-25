#pragma once

typedef void (*RtLogSink)(const char *message);

void rt_log_init(RtLogSink sink);
void rt_log_info(const char *msg);
void rt_log_warn(const char *msg);
void rt_log_error(const char *msg);
