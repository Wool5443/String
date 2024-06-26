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
    std::size_t  m_length;
    Utils::Error m_error;
public:
             Array() = delete;
    explicit Array(std::size_t length);
             Array(const Array& other);
             Array(Array&& other) noexcept;
             ~Array();
public:
    Array& operator=(const Array& other);
    Array& operator=(Array&& other) noexcept;
    T&     operator[](std::size_t index) noexcept { return m_buf[index]; }
public:
    inline std::size_t         Length() const   noexcept { return m_length; }
    inline Utils::Error&       Error()        & noexcept { return m_error; }
    inline const Utils::Error& Error()  const & noexcept { return m_error; }
};
}


#endif
