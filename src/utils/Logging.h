#ifndef A_DARK_DISCOMFORT_LOGGING_H
#define A_DARK_DISCOMFORT_LOGGING_H

#include "Macros.h"
#include "StringHelpers.h"
#include "../systems/entity/Entity.h"
#include <iostream>
#include <utility>
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
#define _CC_LOG_FMT_STR CONCAT_SYMBOL(_fmt_str_, __LINE__)
#define _CC_LOG(log_level, colour_macro, message, ...) \
    static Logging::VariantContainer _CC_LOG_VRNT_ARR[] { __VA_ARGS__ }; \
    static size_t _CC_LOG_VRNT_ARR_SZE = sizeof(_CC_LOG_VRNT_ARR)/Logging::VARIANT_SIZE; \
    static std::string _CC_LOG_FMT_STR (_CC_LOG_MSG_FMT(log_level, colour_macro, message)); \
    Logging::VariantFormat(_CC_LOG_FMT_STR, _CC_LOG_VRNT_ARR, _CC_LOG_VRNT_ARR_SZE); \
    std::cout << _CC_LOG_FMT_STR << std::endl;
#define DEFINE_VARIANT_TYPE(type, tranform) \
    VariantContainer(type) : data(tranform) {} // NOLINT(google-explicit-constructor)

namespace Logging
{

    /**
     * A container class for print variant types
     */
    class VariantContainer
    {
    public:

        // 'Structors

        DEFINE_VARIANT_TYPE(const std::string& data, data);
        DEFINE_VARIANT_TYPE(const char* data, data);
        DEFINE_VARIANT_TYPE(const int& data, std::to_string(data));
        DEFINE_VARIANT_TYPE(const float& data, std::to_string(data));
        DEFINE_VARIANT_TYPE(const double& data, std::to_string(data));
        DEFINE_VARIANT_TYPE(const size_t& data, std::to_string(data));
        DEFINE_VARIANT_TYPE(const Entity& data,data.GetName());
        DEFINE_VARIANT_TYPE(const raylib::Vector3& data,
                            "Vector3(" + StringHelpers::VectorToString(data) + ")");

        /**
         * Returns the data held by the variant container
         * @return the data string as a const reference
         */
        const std::string& GetString() const
        {
            return data;
        };

    private:

        // Private fields

        /**
         * String data held by the object
         */
        std::string data;
    };

    // Define constants
    static constexpr const size_t VARIANT_SIZE = sizeof(VariantContainer);
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
    static void VariantFormat(std::string& text, const VariantContainer* variantItems, const size_t& size)
    {
        size_t cursor(0);
        for (size_t i(0); i < size; i++)
        {
            // Find the point of replacement from the last known cursor position
            cursor = text.find(REPLACE_STRING, cursor);
            if (cursor == std::string::npos) break;

            // Replace the format pattern with the current variant item
            const std::string& item = variantItems[i].GetString();
            text.replace(cursor, REPLACE_STRING_SIZE, item);

            // Move cursor to current position
            cursor += item.length();
        }
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
