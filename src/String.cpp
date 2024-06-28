#include <cmath>
#include <cctype>
#include <cstring>
#include "String.hpp"

namespace
{
    const char* WHITE_SPACE = " \n\t\v\f\r";

    inline std::size_t calcCapacity(std::size_t hintLength)
    {
        std::size_t capacity = Buffers::StringBuffer::DEFAULT_STRING_CAPACITY;

        while (capacity <= hintLength)
            capacity *= Buffers::StringBuffer::STRING_GROW_FACTOR;

        return capacity;
    }

    std::size_t countWords(const Containers::String& string, const char* delimeters, std::size_t delimLength);
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

Buffers::Array<Containers::String> Containers::String::Split()
{
    return this->Split(WHITE_SPACE);
}

Buffers::Array<Containers::String> Containers::String::Split(const char* delimeters)
{
    char* buf = this->Buffer();

    std::size_t wordsCount = countWords(*this, delimeters, strlen(delimeters));

    Buffers::Array<String> wordsArray{wordsCount};

    const char* token = strtok(buf, delimeters);

    std::size_t i = 0;

    while (token)
    {
        wordsArray[i++] = token;
        token = strtok(nullptr, delimeters);
    }

    wordsArray.Length() = i;

    return wordsArray;
}

Buffers::Array<Containers::String> Containers::String::Split(const String& delimeters)
{
    return this->Split(delimeters.Buffer());
}

namespace
{
std::size_t countWords(const Containers::String& string, const char* delimeters, std::size_t delimLength)
{
    if (!delimeters) return Utils::SIZET_POISON;

    const char* buffer = string.Buffer();

    std::size_t words  = 1;

    for (std::size_t i = 0, length = string.Length(); i < length; i++)
    {
        for (std::size_t j = 0; j < delimLength; j++)
        {
            if (buffer[i] == delimeters[j])
            {
                words++;
                break;
            }
        }
    }

    return words;
}
}

Utils::Error Containers::String::Filter(const char* filter)
{
    SoftAssert(filter, Utils::ErrorCode::ERROR_NULLPTR);

    char*       writePtr = this->Buffer();
    const char* readPtr  = writePtr;

    while (*readPtr)
    {
        char c = *readPtr++;

        if (!std::strchr(filter, c))
            *writePtr++ = c;
    }

    *writePtr = '\0';

    return Error();
}

Utils::Error Containers::String::Filter(const String& filter)
{
    return this->Filter(filter.Buffer());
}
