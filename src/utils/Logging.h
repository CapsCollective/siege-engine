#ifndef A_DARK_DISCOMFORT_LOGGING_H
#define A_DARK_DISCOMFORT_LOGGING_H

#include "Macros.h"
#include "StringHelpers.h"
#include <iostream>
#include <variant>
#include <vector>

// Define terminal colour wrapping macros
#define _CC_LOG_WRAP_TEXT_RED(text) "\x1B[31m" text "\033[0m"
#define _CC_LOG_WRAP_TEXT_YELLOW(text) "\x1B[33m" text "\033[0m"
#define _CC_LOG_WRAP_TEXT_GREY(text) "\x1B[37m" text "\033[0m"

// Define logging helper macros
#define _CC_LOG_LINE_LOC __FILE__ ":" TOSTRING(__LINE__)
#define _CC_LOG_MSG_FMT(log_level, colour_macro, message) colour_macro(log_level " at [" _CC_LOG_LINE_LOC "] " message)
#define _CC_LOG_VRNT_ARR CONCAT_SYMBOL(_vrnt_arr_, __LINE__)
#define _CC_LOG_VRNT_ARR_SZE CONCAT_SYMBOL(_vrnt_arr_sze_, __LINE__)
#define _CC_LOG(log_level, colour_macro, message, ...) \
    static Logging::Variant _CC_LOG_VRNT_ARR[] { __VA_ARGS__ }; \
    static size_t _CC_LOG_VRNT_ARR_SZE = sizeof(_CC_LOG_VRNT_ARR)/Logging::VARIANT_SIZE; \
    Logging::VariantFormatPrint( \
        _CC_LOG_MSG_FMT(log_level, colour_macro, message), _CC_LOG_VRNT_ARR, _CC_LOG_VRNT_ARR_SZE)

namespace Logging
{
    // Define types
    typedef std::variant<std::string, int, float> Variant;

    // Define constants
    static constexpr const size_t VARIANT_SIZE = sizeof(Logging::Variant);
    static constexpr const char REPLACE_STRING[] = "{}";
    static constexpr const size_t REPLACE_STRING_SIZE = sizeof(REPLACE_STRING) - 1;

    /**
     * Allows for the format printing of strings with a variant
     * list of format arguments.
     * @param text - the text to format, using "{}" to define
     *               variant replacement
     * @param variantItems - an array of variant items to format
     *                       the text with
     * @param size - the size of variantItems
     */
    static void VariantFormatPrint(const std::string& text, const Logging::Variant* variantItems, const size_t& size)
    {
        std::string fmt(text);
        for (size_t i = 0; i < size; i++)
        {
            std::string str;
            const Variant& item = variantItems[i];
            if (std::holds_alternative<std::string>(item)) str = std::get<std::string>(item);
            else if (std::holds_alternative<int>(item)) str = std::to_string(std::get<int>(item));
            else if (std::holds_alternative<float>(item)) str = std::to_string(std::get<float>(item));

            size_t removalPoint = fmt.find(REPLACE_STRING);
            std::cout << fmt.substr(0, removalPoint) << str;
            fmt.erase(0, removalPoint+REPLACE_STRING_SIZE);
        }
        std::cout << fmt << std::endl;
    }
}

// Determine log level
#ifdef CC_LOG_LEVEL
#if CC_LOG_LEVEL==0
#define _CC_LOG_DEFINE_ERROR
#elif CC_LOG_LEVEL==1
#define _CC_LOG_DEFINE_ERROR
#define _CC_LOG_DEFINE_WARNING
#elif CC_LOG_LEVEL==2
#define _CC_LOG_DEFINE_ERROR
#define _CC_LOG_DEFINE_WARNING
#define __CC_LOG_DEFINE_INFO
#endif
#endif

// Define error logging macro
#ifdef _CC_LOG_DEFINE_ERROR
#define CC_LOG_ERROR(message, ...) _CC_LOG("ERROR", _CC_LOG_WRAP_TEXT_RED, message, ##__VA_ARGS__)
#else
#define CC_LOG_ERROR(...)
#endif

// Define warning logging macro
#ifdef _CC_LOG_DEFINE_WARNING
#define CC_LOG_WARNING(message, ...) _CC_LOG("WARNING", _CC_LOG_WRAP_TEXT_YELLOW, message, ##__VA_ARGS__)
#else
#define CC_LOG_WARNING(...)
#endif

// Define info logging macro
#ifdef __CC_LOG_DEFINE_INFO
#define CC_LOG_INFO(message, ...) _CC_LOG("INFO", _CC_LOG_WRAP_TEXT_GREY, message, ##__VA_ARGS__)
#else
#define CC_LOG_INFO(...)
#endif

#endif //A_DARK_DISCOMFORT_LOGGING_H
