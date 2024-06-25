#include "StringBuffer.hpp"
#include "StringBufferSettings.hpp"

using namespace StringBufferSettings;

Buffers::StringBuffer::StringBuffer(std::size_t capacity)
    : m_buf(new char[capacity]), m_capacity(capacity),
      m_length(0)
{
    if (!m_buf)
        m_error = CREATE_ERROR(Utils::ErrorCode::ERROR_NO_MEMORY);
}

Buffers::StringBuffer::StringBuffer()
    : StringBuffer(DEFAULT_STRING_CAPACITY) {}
