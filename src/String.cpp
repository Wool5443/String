#include <cmath>
#include <cctype>
#include <cstring>
#include "String.hpp"

namespace
{
    inline std::size_t calcCapacity(std::size_t hintLength)
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
    std::memcpy(m_buf.Buffer(), string, length);
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

    std::memcpy(this->Buffer() + this->Length(), other.Buffer(), other.Length());

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

std::size_t Containers::String::Find(char c)
{
    const char* found = std::strchr(this->Buffer(), c);

    if (!found) return Utils::SIZET_POISON;

    return found - this->Buffer();
}

std::size_t Containers::String::Find(const char* substr)
{
    const char* found = std::strstr(*this, substr);

    if (!found) return Utils::SIZET_POISON;

    return found - this->Buffer();
}

std::size_t Containers::String::Find(const String& substr)
{
    return this->Find(substr.Buffer());
}

std::size_t Containers::String::Count(char c)
{
    const char* found = std::strchr(this->Buffer(), c);

    if (!found) return Utils::SIZET_POISON;

    std::size_t counter = 0;

    while (found)
    {
        counter++;
        found = std::strchr(found + 1, c);
    }

    return counter;
}

std::size_t Containers::String::Count(const char* substr)
{
    const char* found = std::strstr(this->Buffer(), substr);

    if (!found) return Utils::SIZET_POISON;

    std::size_t counter = 0;

    while (found)
    {
        counter++;
        found = std::strstr(found + 1, substr);
    }

    return counter;
}

std::size_t Containers::String::Count(const String& substr)
{
    return this->Count(substr.Buffer());
}
