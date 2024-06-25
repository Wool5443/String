#pragma once

#include "StringBuffer.hpp"

namespace Containers
{
class String
{
private:
    Buffers::StringBuffer m_buf;
public:
    String();
    String(std::size_t capacity);
    String(const char* string);
    String(const char* string, std::size_t length);

    operator Utils::Error() { return m_buf; }
    operator char*()        { return m_buf; }
    operator bool()         { return m_buf; }
};
}