#include "StringBuffer.hpp"

Buffers::StringBuffer::StringBuffer(std::size_t capacity)
    : StringBuffer(capacity, 0) {}

Buffers::StringBuffer::StringBuffer(std::size_t capacity, std::size_t length)
    : m_buf(new char[capacity]{}), m_capacity(capacity),
      m_length(length), m_error()
{
    if (!m_buf)
        m_error = CREATE_ERROR(Utils::ErrorCode::ERROR_NO_MEMORY);
}

Buffers::StringBuffer::~StringBuffer()
{
    if (m_error) return;
    delete[] m_buf;
}

Buffers::StringBuffer::StringBuffer()
    : StringBuffer(DEFAULT_STRING_CAPACITY) {}

Buffers::StringBuffer::StringBuffer(const StringBuffer& other)
    : m_buf(new char[other.m_capacity]{}), m_capacity(other.m_capacity),
      m_length(other.m_length), m_error()
{
    if (!m_buf)
    {
        m_error = CREATE_ERROR(Utils::ErrorCode::ERROR_NO_MEMORY);
        return;
    }
    std::copy(other.m_buf, other.m_buf + m_length, m_buf);
}

Buffers::StringBuffer::StringBuffer(StringBuffer&& other)
    : m_buf(other.m_buf), m_capacity(other.m_capacity),
      m_length(other.m_length), m_error()
{
    other.m_buf = nullptr;
}

Buffers::StringBuffer& Buffers::StringBuffer::operator=(const StringBuffer& other)
{
    if (this == &other) return *this;

    delete[] m_buf;

    m_capacity = other.m_capacity;
    m_length   = other.m_length;

    m_buf      = new char[m_capacity]{};
    if (!m_buf) 
    {
        m_error = CREATE_ERROR(Utils::ErrorCode::ERROR_NO_MEMORY);
        return *this;
    }

    std::copy(other.m_buf, other.m_buf + m_length, m_buf);

    return *this;
}

Buffers::StringBuffer& Buffers::StringBuffer::operator=(StringBuffer&& other)
{
    std::swap(m_buf, other.m_buf);
    m_capacity = other.m_capacity;
    m_length   = other.m_length;
    m_error    = other.m_error;

    return *this;
}
