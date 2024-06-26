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
    static const std::size_t DEFAULT_STRING_CAPACITY = 8;
    static const std::size_t STRING_GROW_FACTOR      = 2;
public:
             StringBuffer();
    explicit StringBuffer(std::size_t capacity);
             StringBuffer(std::size_t capacity, std::size_t length);
             StringBuffer(const StringBuffer& other);
             StringBuffer(StringBuffer&& other);
             ~StringBuffer();
public:
    StringBuffer& operator=(const StringBuffer& other);
    StringBuffer& operator=(StringBuffer&& other);
public:
    inline char*               __attribute__((always_inline)) Buffer()           { return m_buf; }
    inline const char*         __attribute__((always_inline)) Buffer()   const   { return m_buf; }
    inline std::size_t         __attribute__((always_inline)) Capacity() const   { return m_capacity; }
    inline std::size_t         __attribute__((always_inline)) Length()   const   { return m_length; }
    inline Utils::Error&       __attribute__((always_inline)) Error()          & { return m_error; }
    inline const Utils::Error& __attribute__((always_inline)) Error()    const & { return m_error; }
};
}
