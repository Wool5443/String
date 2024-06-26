#include <cmath>
#include <cctype>
#include <cstring>
#include "String.hpp"

namespace
{
    inline __attribute__((always_inline)) std::size_t calcCapacity(std::size_t hintLength)
    {
        std::size_t capacity = Buffers::StringBuffer::DEFAULT_STRING_CAPACITY;

        while (capacity <= hintLength)
            capacity *= Buffers::StringBuffer::STRING_GROW_FACTOR;

        return capacity;
    }
}

Containers::String::String(std::size_t capacity)
    : m_buf(capacity) {}

Containers::String::String(std::size_t capacity, std::size_t length)
    : m_buf(capacity, length) {}

Containers::String::String()
    : m_buf() {}

Containers::String::String(const char* string, std::size_t length)
    : String(calcCapacity(length), length)
{
    std::copy(string, string + length, m_buf.Buffer());
}

Containers::String::String(const char* string)
    : String(string, strlen(string)) {}

Containers::String& Containers::String::operator+=(const String& other)
{
    std::size_t newLength = this->Length() + other.Length();

    if (newLength > this->Capacity())
    {
        this->realloc(newLength);
        if (this->Error()) return *this;
    }

    std::copy(other.Buffer(), other.Buffer() + other.Length(), this->Buffer() + this->Length());

    return *this;
}

void Containers::String::realloc(std::size_t newLength)
{
    std::size_t newCapacity = this->Capacity() / Buffers::StringBuffer::STRING_GROW_FACTOR;

    if (newLength > this->Capacity())
        newCapacity = calcCapacity(newLength);
    else if (newLength >= newCapacity)
        return;

    Buffers::StringBuffer newBuf{ newCapacity, this->Length() };
    if (newBuf.Error())
    {
        this->Error() = newBuf.Error();
        return;
    }

    char* oldBuf = this->Buffer();

    std::copy(oldBuf, oldBuf + this->Length(), newBuf.Buffer());

    this->m_buf = std::move(newBuf);
}
