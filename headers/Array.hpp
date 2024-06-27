#ifndef MY_SPLIT_STRING_BUFFER_HPP
#define MY_SPLIT_STRING_BUFFER_HPP

#include "Utils.hpp"

namespace Buffers
{
template<typename T>
class Array
{
private:
    T*           m_buf;
    std::size_t  m_capacity;
    std::size_t  m_length;
    Utils::Error m_error = Utils::Error();
public:
Array() = delete;

explicit Array(std::size_t capacity)
    : m_buf(new T[capacity]), m_capacity(capacity), m_length(0)
{
    if (!m_buf)
        m_error = CREATE_ERROR(Utils::ErrorCode::ERROR_NO_MEMORY);
}

Array(const Array& other)
    : m_buf(new T[other.m_capacity]), m_capacity(other.m_capacity), m_length(other.m_length)
{
    if (!m_buf)
    {
        m_error = CREATE_ERROR(Utils::ErrorCode::ERROR_NO_MEMORY);
        return;
    }

    std::copy(other.m_buf, other.m_buf + other.m_length, m_buf);
}

Array(Array&& other) noexcept
    : m_buf(other.m_buf), m_capacity(other.m_capacity), m_length(other.m_length)
{
    other.m_buf = nullptr;
}

~Array()
{
    delete[] m_buf;
}
public:
Array& operator=(const Array& other)
{
    if (this == &other) return *this;

    delete[] m_buf;

    m_capacity = other.m_capacity;
    m_length   = other.m_length;
    m_buf      = new T[m_length];

    if (!m_buf)
    {
        m_error = CREATE_ERROR(Utils::ErrorCode::ERROR_NO_MEMORY);
        return *this;
    }

    std::copy(other.m_buf, other.m_buf + m_length, m_buf);

    return *this;
}

Array& operator=(Array&& other) noexcept
{
    std::swap(m_buf, other.m_buf);
    m_capacity = other.m_capacity;
    m_length   = other.m_length;

    return *this;
}

T& operator[](std::size_t index) noexcept { return m_buf[index]; }
public:
    inline std::size_t&        Length()       & noexcept { return m_length; }
    inline const std::size_t&  Length() const & noexcept { return m_length; }
    inline Utils::Error&       Error()        & noexcept { return m_error; }
    inline const Utils::Error& Error()  const & noexcept { return m_error; }
};
}

#endif
