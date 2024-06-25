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
private:
    StringBuffer(std::size_t capacity);
public:
    StringBuffer();
};
}
