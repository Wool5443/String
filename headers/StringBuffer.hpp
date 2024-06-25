#pragma once

#include "Utils.hpp"

namespace Buffers
{
class StringBuffer
{
private:
    char*        m_buf;
    std::size_t  m_capacity;
    std::size_t  m_length;
    Utils::Error m_error;
public:
    StringBuffer();
    StringBuffer(std::size_t capacity);
    StringBuffer(const StringBuffer& other);
    StringBuffer(StringBuffer&& other);
    ~StringBuffer();
public:
    StringBuffer& operator=(const StringBuffer& other);
    StringBuffer& operator=(StringBuffer&& other);
public:
    operator bool()         { return m_error; }
    operator Utils::Error() { return m_error; }
    operator char*()        { return m_buf; }
};
}
