#include "Array.hpp"

template<typename T>
Buffers::Array<T>::Array(std::size_t length)
    : m_buf(new T[length]), m_length(length)
{
    if (!m_buf)
        m_error = CREATE_ERROR(Utils::ErrorCode::ERROR_NO_MEMORY);
}

template<typename T>
Buffers::Array<T>::Array(const Array<T>& other)
    : m_buf(new T[other.m_length]), m_length(other.m_length)
{
    if (!m_buf)
    {
        m_error = CREATE_ERROR(Utils::ErrorCode::ERROR_NO_MEMORY);
        return;
    }

    std::copy(other.m_buf, other.m_buf + other.m_length, m_buf);
}

template<typename T>
Buffers::Array<T>::Array(Array<T>&& other) noexcept
    : m_buf(other.m_buf), m_length(other.m_length)
{
    other.m_buf = nullptr;
}

template<typename T>
Buffers::Array<T>::~Array()
{
    delete[] m_buf;
}

template<typename T>
Buffers::Array<T>& Buffers::Array<T>::operator=(const Array<T>& other)
{
    if (this == &other) return *this;

    delete[] m_buf;

    m_length = other.m_length;
    m_buf    = new T[m_length];

    if (!m_buf)
    {
        m_error = CREATE_ERROR(Utils::ErrorCode::ERROR_NO_MEMORY);
        return *this;
    }

    std::copy(other.m_buf, other.m_buf + m_length, m_buf);

    return *this;
}

template<typename T>
Buffers::Array<T>& Buffers::Array<T>::operator=(Array<T>&& other) noexcept
{
    std::swap(m_buf, other.m_buf);
    m_length = other.m_length;

    return *this;
}
