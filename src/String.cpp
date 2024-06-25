#include <cmath>
#include <cctype>
#include <cstring>
#include "String.hpp"
#include "StringBufferSettings.hpp"

namespace
{
    inline __attribute__((always_inline)) std::size_t calcCapacity(std::size_t hintLength)
    {
        std::size_t capacity = StringBufferSettings::DEFAULT_STRING_CAPACITY;

        while (capacity <= hintLength)
            capacity *= StringBufferSettings::STRING_GROW_FACTOR;

        return capacity;
    }
}

Containers::String::String(std::size_t capacity)
    : m_buf(capacity) {}

Containers::String::String()
    : m_buf() {}

Containers::String::String(const char* string, std::size_t length)
    : String(calcCapacity(length))
{
    std::copy(string, string + length, (char*)m_buf);
}

Containers::String::String(const char* string)
    : String(string, strlen(string)) {}
