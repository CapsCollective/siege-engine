#ifndef A_DARK_DISCOMFORT_LOGGING_H
#define A_DARK_DISCOMFORT_LOGGING_H

#include <iostream>
#include <utility>
#include <variant>
#include <vector>

#include "../entity/Entity.h"
#include "Macros.h"
#include "String.h"

// Define terminal colour wrapping macros
#define _CC_LOG_COLOUR_RED "31"
#define _CC_LOG_COLOUR_YELLOW "33"
#define _CC_LOG_COLOUR_GREY "37"
#define _CC_LOG_WRAP_TEXT(colour, text) "\x1B[" colour "m" text "\033[0m"

// Define logging helper macros
#define _CC_LOG_LINE_LOC __FILE__ ":" TOSTRING(__LINE__)
#define _CC_LOG_MSG_FMT(ll, col, msg) _CC_LOG_WRAP_TEXT(col, ll " at [" _CC_LOG_LINE_LOC "] " msg)
#define _CC_LOG_VRNT_ARR CONCAT_SYMBOL(_vrnt_arr_, __LINE__)
#define _CC_LOG_VRNT_ARR_SZE CONCAT_SYMBOL(_vrnt_arr_sze_, __LINE__)
#define _CC_LOG_FMT_STR CONCAT_SYMBOL(_fmt_str_, __LINE__)
#define _CC_LOG(log_level, colour, message, ...)                                              \
    {                                                                                         \
        Logging::VariantContainer _CC_LOG_VRNT_ARR[] {__VA_ARGS__};                           \
        static size_t _CC_LOG_VRNT_ARR_SZE(sizeof(_CC_LOG_VRNT_ARR) / Logging::VARIANT_SIZE); \
        String _CC_LOG_FMT_STR(_CC_LOG_MSG_FMT(log_level, colour, message));                  \
        Logging::VariantFormat(_CC_LOG_FMT_STR, _CC_LOG_VRNT_ARR, _CC_LOG_VRNT_ARR_SZE);      \
        std::cout << _CC_LOG_FMT_STR << std::endl;                                            \
    }
#define DEFINE_VARIANT_TYPE(type, transform) \
    VariantContainer(type) : data(transform) {}

// Don't compile logging if unused
#ifdef CC_LOG_LEVEL
#ifndef CC_ENABLE_LOGGING
#define CC_ENABLE_LOGGING
#endif
#endif

#ifdef CC_ENABLE_LOGGING
namespace Logging
{

/**
 * A container class for print variant types
 */
class VariantContainer
{
public:

    // 'Structors

    DEFINE_VARIANT_TYPE(const String& data, data);
    DEFINE_VARIANT_TYPE(const char* data, data);
    DEFINE_VARIANT_TYPE(const char data, data);
    DEFINE_VARIANT_TYPE(const int& data, String::FromInt(data));
    DEFINE_VARIANT_TYPE(const float& data, String::FromFloat(data));
    DEFINE_VARIANT_TYPE(const double& data, String::FromDouble(data));
    DEFINE_VARIANT_TYPE(const long& data, String::FromLong(data));
    DEFINE_VARIANT_TYPE(bool data, data ? "true" : "false");
    DEFINE_VARIANT_TYPE(const Entity& data, data.GetName());
    DEFINE_VARIANT_TYPE(const GenerationalIndex& data, data.ToString());
    DEFINE_VARIANT_TYPE(const Vec3& data, "Vector3(" + data.ToString() + ")");

    /**
     * Returns the data held by the variant container
     * @return the data string as a const reference
     */
    const String& GetString() const
    {
        return data;
    };

private:

    // Private fields

    /**
     * String data held by the object
     */
    String data;
};

// Define constants
static constexpr const size_t VARIANT_SIZE = sizeof(VariantContainer);
static constexpr const char REPLACE_STRING[] = "{}";

/**
 * Allows for the format printing of strings with a variant
 * list of format arguments.
 * @param text - the text to format, using "{}" to define
 *               variant replacement
 * @param variantItems - an array of variant items to format
 *                       the text with
 * @param size - the size of variantItems
 */
static void VariantFormat(String& text, const VariantContainer* variantItems, const size_t& size)
{
    size_t cursor(0);
    for (size_t i(0); i < size; i++)
    {
        // Find the point of replacement from the last known cursor position
        cursor = text.Find(REPLACE_STRING, (int) cursor);
        if (cursor == -1) break;

        // Replace the format pattern with the current variant item
        const String& item = variantItems[i].GetString();
        text.Replace(REPLACE_STRING, item);

        // Move cursor to current position
        cursor += item.Size();
    }
}
} // namespace Logging
#endif

// Determine log level
#ifdef CC_LOG_LEVEL
#if CC_LOG_LEVEL == 0
#define _CC_LOG_DEFINE_ERROR
#elif CC_LOG_LEVEL == 1
#define _CC_LOG_DEFINE_ERROR
#define _CC_LOG_DEFINE_WARNING
#elif CC_LOG_LEVEL == 2
#define _CC_LOG_DEFINE_ERROR
#define _CC_LOG_DEFINE_WARNING
#define __CC_LOG_DEFINE_INFO
#endif
#endif

// Define error logging macro
#ifdef _CC_LOG_DEFINE_ERROR
#define CC_LOG_ERROR(msg, ...) _CC_LOG("ERROR", _CC_LOG_COLOUR_RED, msg, ##__VA_ARGS__)
#else
#define CC_LOG_ERROR(...)
#endif

// Define warning logging macro
#ifdef _CC_LOG_DEFINE_WARNING
#define CC_LOG_WARNING(msg, ...) _CC_LOG("WARNING", _CC_LOG_COLOUR_YELLOW, msg, ##__VA_ARGS__)
#else
#define CC_LOG_WARNING(...)
#endif

// Define info logging macro
#ifdef __CC_LOG_DEFINE_INFO
#define CC_LOG_INFO(msg, ...) _CC_LOG("INFO", _CC_LOG_COLOUR_GREY, msg, ##__VA_ARGS__)
#else
#define CC_LOG_INFO(...)
#endif

#endif // A_DARK_DISCOMFORT_LOGGING_H
