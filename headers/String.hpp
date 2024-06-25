#pragma once

#include "StringBuffer.hpp"

namespace Containers
{
class String
{
private:
    Buffers::StringBuffer m_buf;
    String(std::size_t capacity, std::size_t length);
    void realloc(std::size_t newLength);
public:
    String();
    String(std::size_t capacity);
    String(const char* string);
    String(const char* string, std::size_t length);

    inline char* __attribute__((always_inline)) Buffer()           
    {
        return m_buf.Buffer();
    }
    inline const char* __attribute__((always_inline)) Buffer() const
    {
        return m_buf.Buffer();
    }
    inline std::size_t __attribute__((always_inline)) Capacity() const
    {
        return m_buf.Capacity();
    }
    inline std::size_t __attribute__((always_inline)) Length() const
    {
        return m_buf.Length();
    }
    inline Utils::Error& __attribute__((always_inline)) Error() &
    {
        return m_buf.Error();
    }
    inline const Utils::Error& __attribute__((always_inline)) Error() const &
    {
        return m_buf.Error();
    }

    operator Utils::Error() { return this->Error(); }
    operator char*()        { return this->Buffer(); }
    operator bool()         { return this->Buffer(); }

    String& operator+=(const String& other);
};
}