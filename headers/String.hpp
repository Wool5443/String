#ifndef MY_STRING_HPP
#define MY_STRING_HPP

#include <iostream>

#include "StringBuffer.hpp"
#include "Array.hpp"

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
public:
    Buffers::Array<String> Split();
    Buffers::Array<String> Split(const char* delimeters);
    Buffers::Array<String> Split(const String& delimeters);
public:
    std::size_t Find(char c);
    std::size_t Find(const char* substr);
    std::size_t Find(const String& substr);
    std::size_t Count(char c);
    std::size_t Count(const char* substr);
    std::size_t Count(const String& substr);
public:
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

    operator char*()             { return this->Buffer(); }
    operator const char*() const { return this->Buffer(); }
    operator bool()        const { return this->Buffer(); }

    String& operator+=(const String& other);

    friend std::ostream& operator<<(std::ostream& out, const String& string)
    {
        out << string.Buffer();
        return out;
    }

};
}

#endif
