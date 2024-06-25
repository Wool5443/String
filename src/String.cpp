#include <cmath>
#include <cctype>
#include <cstring>
#include "String.hpp"
#include "StringBufferSettings.hpp"

namespace
{
    inline __attribute__((always_inline)) std::size_t calcCapacity(std::size_t hintLength)
    {
        std::size_t capacity = StringBufferSettings::DEFAULT_STRING_CAPACITY;

        while (capacity <= hintLength)
            capacity *= StringBufferSettings::STRING_GROW_FACTOR;

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

<<<<<<< HEAD
void _destructor(String& string) noexcept
{
    CHECK_ERROR(string.error);

    free(string.buf);
    string.buf      = nullptr;
    string.capacity = 0;
    string.length   = 0;
}

Error String::Append(char chr) noexcept
{
    RETURN_ERROR(_realloc(*this, this->length + 1));

    this->buf[this->length++] = chr;

    return Error();
}

Error String::Append(const char* string) noexcept
{
    if (!string)
        return Error();
    return _append(*this, string, strlen(string));
}

Error String::Append(const String& string) noexcept
{
    return _append(*this, string.buf, string.length);
}

Error _append(String& string, const char* add, size_t length)
{
    CHECK_ERROR(string.error, string.error);

    SoftAssert(add, ERROR_NULLPTR);

    RETURN_ERROR(_realloc(string, string.length + length));

    strncpy(string.buf + string.length, add, length);
    string.length += length;

    return Error();
}

String String::Concat(const char* string) const noexcept
{
    if (!string)
        return *this;

    return _concat(*this, string, strlen(string));
}

String String::Concat(const String& string) const noexcept
{
    if (!string.buf)
        return *this;

    return _concat(*this, string.buf, string.length);
}

static String _concat(const String& string, const char* add, size_t length)
{
    CHECK_ERROR(string.error, BAD_STRING);

    String newString = string;

    if (newString.error)
    {
        return newString;
    }

    _append(newString, add, length);

    return newString;
}

size_t String::Count(char character) const noexcept
{
    size_t count = 0;
    for (size_t i = 0; i < this->length; i++)
        if (this->buf[i] == character)
            count++;

    return count;
}

size_t String::Count(const char* string) const noexcept
{
    return _countStr(this->buf, string);
}

size_t String::Count(const String& string) const noexcept
{
    return _countStr(this->buf, string.buf);
}

static size_t _countStr(const char* string, const char* needle)
{
    if (!string || !needle)
        return SIZET_POISON;

    size_t      count = 0;
    const char* found = strstr(string, needle);

    while (found)
    {
        count++;
        found = strstr(found, needle);
    }

    return count;
}

static size_t _countWords(const String& string, const char* delimiters, size_t length)
{
    CHECK_ERROR(string.error, SIZET_POISON);

    if (!delimiters)
        return SIZET_POISON;

    size_t wordsCount = 1;

    for (size_t i = 0; i < string.length; i++)
    {
        for (size_t j = 0; j < length; j++)
        {
            if (string.buf[i] == delimiters[j])
            {
                wordsCount++;
                break;
            }
        }
    }

    return wordsCount;
}

SplitString::SplitString() noexcept
    : words(nullptr), wordsCount(SIZET_POISON) {}

SplitString::SplitString(String* words, size_t wordsCount) noexcept
    : words(words), wordsCount(wordsCount) {}

SplitString::~SplitString() noexcept
{
    for (size_t i = 0; i < this->wordsCount; i++)
        this->words[i].~String();

    free(this->words);
    this->words      = nullptr;
    this->wordsCount = 0;
}

SplitStringResult String::Split() noexcept
{
    return _split(*this, SPACE_CHARS, SPACE_CHARS_LENGTH);
}

SplitStringResult String::Split(const char* delimiters) noexcept
{
    return _split(*this, delimiters, strlen(delimiters));
}

SplitStringResult String::Split(const String& delimiters) noexcept
{
    return _split(*this, delimiters.buf, delimiters.length);
}

static SplitStringResult _split(String& string, const char* delimiters, size_t length)
{
    CHECK_ERROR(string.error, { {}, string.error });

    if (!delimiters) return { { &string, 1 }, Error() };

    size_t wordsCount = _countWords(string, delimiters, length);
    if (wordsCount == 1) return { { &string, 1 }, Error() };

    String* words = (String*)calloc(wordsCount, sizeof(*words));

    char* currentToken = strtok(string.buf, delimiters);
    size_t i = 0;
    while (currentToken)
    {
        size_t length = strlen(currentToken);
        RETURN_ERROR_RESULT(_create(words[i], length,
                                    _calcCapacity(DEFAULT_STRING_CAPACITY, length), currentToken),
                                    {});

        currentToken = strtok(nullptr, delimiters);
        i++;
    }

    return { SplitString(words, i), Error() };
}

Error String::Filter() noexcept
{
    return _filter(*this, SPACE_CHARS);
}

Error String::Filter(const char* filter) noexcept
{
    SoftAssert(filter, ERROR_NULLPTR);
    return _filter(*this, filter);
}

Error String::Filter(const String& filter) noexcept
{
    return _filter(*this, filter.buf);
}

static Error _filter(String& string, const char* filter)
{
    CHECK_ERROR(string.error, string.error);

    SoftAssert(filter, ERROR_NULLPTR);

    const char* readPtr  = string.buf;
    char*       writePtr = string.buf;

    while (*readPtr)
    {
        char c = *readPtr++;

        if (!strchr(filter, c))
            *writePtr++ = c;
    }
    *writePtr = '\0';

    return Error();
}

bool String::IsSpaceCharacters() const noexcept
{
    for (size_t i = 0; i < this->length; i++)
        if (!isspace(this->buf[i]))
            return false;
    return true;
}

bool String::IsSpaceCharacters(const char* string)
{
    while (*string && isspace(*string))
        string++;
    
    if (*string == '\0')
        return true;
    return false;
}
=======
Containers::String::String(const char* string)
    : String(string, strlen(string)) {}
>>>>>>> Actually-good-string
