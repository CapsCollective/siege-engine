#ifndef A_DARK_DISCOMFORT_LOGGING_H
#define A_DARK_DISCOMFORT_LOGGING_H

#include "Macros.h"
#include <iostream>

// Define terminal colour wrapping macros
#define _CC_WRAP_TEXT_RED(text) "\x1B[31m" text "\033[0m"
#define _CC_WRAP_TEXT_YELLOW(text) "\x1B[33m" text "\033[0m"
#define _CC_WRAP_TEXT_GREY(text) "\x1B[37m" text "\033[0m"

// Define logging helper macros
#define _CC_LINE_LOC __FILE__ ":" TOSTRING(__LINE__)
// TODO make this use cout
#define _CC_LOG(log_level, colour_macro, message, ...) \
    std::printf(colour_macro(log_level " at [" _CC_LINE_LOC "] " message) "\n", ##__VA_ARGS__)

// Determine log level
#ifdef CC_LOG_LEVEL
#if CC_LOG_LEVEL==0
#define _CC_DEFINE_LOG_ERROR
#elif CC_LOG_LEVEL==1
#define _CC_DEFINE_LOG_ERROR
#define _CC_DEFINE_LOG_WARNING
#elif CC_LOG_LEVEL==2
#define _CC_DEFINE_LOG_ERROR
#define _CC_DEFINE_LOG_WARNING
#define _CC_DEFINE_LOG_INFO
#endif
#endif

// Define error logging macro
#ifdef _CC_DEFINE_LOG_ERROR
#define CC_LOG_ERROR(message, ...) _CC_LOG("ERROR", _CC_WRAP_TEXT_RED, message, ##__VA_ARGS__)
#else
#define CC_LOG_ERROR(...)
#endif

// Define warning logging macro
#ifdef _CC_DEFINE_LOG_WARNING
#define CC_LOG_WARNING(message, ...) _CC_LOG("WARNING", _CC_WRAP_TEXT_YELLOW, message, ##__VA_ARGS__)
#else
#define CC_LOG_WARNING(...)
#endif

// Define info logging macro
#ifdef _CC_DEFINE_LOG_INFO
#define CC_LOG_INFO(message, ...) _CC_LOG("INFO", _CC_WRAP_TEXT_GREY, message, ##__VA_ARGS__)
#else
#define CC_LOG_INFO(...)
#endif

#endif //A_DARK_DISCOMFORT_LOGGING_H
