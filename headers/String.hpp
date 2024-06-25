#pragma once

#include "StringBuffer.hpp"

namespace Containers
{
class String
{
private:
    Buffers::StringBuffer m_buf;
    String(std::size_t capacity, std::size_t length);
public:
    String();
    String(std::size_t capacity);
    String(const char* string);
    String(const char* string, std::size_t length);

    inline char*        __attribute__((always_inline)) Buffer() { return m_buf.Buffer(); }
    inline std::size_t  __attribute__((always_inline)) Length() { return m_buf.Length(); }
    inline Utils::Error __attribute__((always_inline)) Error()  { return m_buf.Error(); }

    operator Utils::Error() { return this->Error(); }
    operator char*()        { return this->Buffer(); }
    operator bool()         { return this->Buffer(); }
};
}