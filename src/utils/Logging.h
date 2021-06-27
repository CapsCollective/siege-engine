#ifndef A_DARK_DISCOMFORT_LOGGING_H
#define A_DARK_DISCOMFORT_LOGGING_H

#include <iostream>

// filename, line
// log level display
#define LINE_LOC __FILE__ ":" TOSTRING(__LINE__)
#define _LOG(log_level, message, ...) std::printf(log_level " at [" LINE_LOC "] " message, ##__VA_ARGS__)
#define LOG_ERROR(message, ...) _LOG("ERROR", message, __VA_ARGS__)
#define LOG_WARNING(message, ...) _LOG("WARNING", message, __VA_ARGS__)
#define LOG_INFO(message, ...) _LOG("INFO", message, __VA_ARGS__)

#endif //A_DARK_DISCOMFORT_LOGGING_H
