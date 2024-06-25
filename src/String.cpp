#include <cmath>
#include <cctype>
#include <cstring>
#include "String.hpp"
#include "StringSettings.hpp"

static const char*  SPACE_CHARS             = " \t\n\r\v\f";
static const std::size_t SPACE_CHARS_LENGTH = 6;

using namespace StringSettings;

#define CHECK_ERROR(error, ...) \
do                              \
{                               \
    if (error)                  \
    {                           \
        error.Print();          \
        return __VA_ARGS__;     \
    }                           \
}   while (0)

namespace
{
static inline __attribute__((always_inline)) std::size_t
_calcCapacity(std::size_t capacity, std::size_t hintLength)
{
    if (hintLength < capacity)
        return capacity;
    return hintLength ?
           capacity * 
           (std::size_t)std::pow(2, 
           (std::size_t)std::log2((double)hintLength / (double)capacity) + 1)
           : DEFAULT_STRING_CAPACITY;
}
}

namespace String
{
String::~String() noexcept
{
    if (m_error) return;
    delete [] m_buf;
}

String::String() noexcept
    : String(DEFAULT_STRING_CAPACITY) {}

String::String(std::size_t capacity) noexcept
    : m_buf(new char[capacity]), m_capacity(capacity),
      m_length(0), m_error()
{
    if (!m_buf)
        m_error = Utils::CREATE_ERROR(Utils::ErrorCode::ERROR_NO_MEMORY);
}

String::String(const char* string) noexcept
    : String(string, std::strlen(string)) {}

String::String(const char* string, std::size_t length) noexcept
    : String(_calcCapacity(DEFAULT_STRING_CAPACITY, length))
{
    if (m_error) return;
    m_length = length;
    std::copy(string, string + length, m_buf);
}

String::String(const String& other) noexcept
    : String(other.m_buf, other.m_length) {}

String::String(String&& other) noexcept
    : m_buf(other.m_buf), m_capacity(other.m_capacity),
      m_length(other.m_length), m_error(other.m_error)
{
}

}

