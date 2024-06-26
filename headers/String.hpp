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

    inline char* Buffer()           
    {
        return m_buf.Buffer();
    }
    inline const char* Buffer() const
    {
        return m_buf.Buffer();
    }
    inline std::size_t Capacity() const
    {
        return m_buf.Capacity();
    }
    inline std::size_t Length() const
    {
        return m_buf.Length();
    }
    inline Utils::Error& Error() &
    {
        return m_buf.Error();
    }
    inline const Utils::Error& Error() const &
    {
        return m_buf.Error();
    }

    operator char*()        { return this->Buffer(); }
    operator bool()         { return this->Buffer(); }

    String& operator+=(const String& other);
};
}
