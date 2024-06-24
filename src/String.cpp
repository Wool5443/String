#include <errno.h>
#include <math.h>
#include <ctype.h>
#include "String.hpp"
#include "StringSettings.hpp"

static const char*  SPACE_CHARS        = " \t\n\r\v\f";
static const size_t SPACE_CHARS_LENGTH = 6;

static inline __attribute__((always_inline)) size_t _calcCapacity(size_t capacity, size_t hintLength)
{
    return hintLength ?
           capacity * 
           (size_t)pow(2, 
           (size_t)log2((double)hintLength / (double)capacity) + 1)
           : DEFAULT_STRING_CAPACITY;
}

static Error             _create(String& string, size_t length, size_t capacity, const char* data);
static Error             _realloc(String& string, size_t hintLength);
static size_t            _countStr(const char*   string, const char* needle);
Error                    _append(String& string, const char* add, size_t length);
static String            _concat(const String& string, const char* add, size_t length);
static size_t            _countWords(const String& string, const char* delimiters, size_t length);
static SplitStringResult _split(String& string, const char* delimiters, size_t length);
static Error             _filter(String& string, const char* filter);

Error String::Create() noexcept
{
    return _create(*this, 0, DEFAULT_STRING_CAPACITY, nullptr);
}

Error String::Create(size_t capacity) noexcept
{
    return _create(*this, 0, capacity, nullptr);
}

Error String::Create(const char* string) noexcept
{
    size_t length = strlen(string);
    return _create(*this, length, _calcCapacity(DEFAULT_STRING_CAPACITY, length), string);
}

Error String::Create(const char* string, size_t length) noexcept
{
    return _create(*this, length, _calcCapacity(DEFAULT_STRING_CAPACITY, length), string);
}

Error String::Create(const String& string) noexcept
{
    return _create(*this, string.length, string.capacity, string.buf);
}

static Error _create(String& string, size_t length, size_t capacity, const char* data)
{
    string.buf = (char*)calloc(string.capacity, 1);

    if (!string.buf)
    {
        string.error = CREATE_ERROR(ERROR_NO_MEMORY);
        return string.error;
    }

    string.capacity = capacity;
    string.length   = length;

    if (data)
        memcpy(string.buf, data, length);

    string.error = Error();
    return Error();
}

static Error _realloc(String& string, size_t hintLength)
{
    if (hintLength < string.capacity)
        return Error();

    size_t newCapacity  = _calcCapacity(string.capacity, hintLength);

    char* newBuf = (char*)calloc(newCapacity, 1);
    if (!newBuf)
        return CREATE_ERROR(ERROR_NO_MEMORY);
    if (string.buf)
        memcpy(newBuf, string.buf, string.capacity);

    free(string.buf);
    string.buf      = newBuf;
    string.capacity = newCapacity;

    return Error();
}

void String::Destructor() noexcept
{
    free(this->buf);
    this->buf       = nullptr;
    this->capacity  = 0;
    this->length    = 0;
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
    String newString = string;

    if (newString.error)
    {
        return newString;
    }

    memcpy(newString.buf + string.length, add, length);

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

void SplitString::Destructor()
{
    for (size_t i = 0; i < this->wordsCount; i++)
        this->words[i].Destructor();

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
    if (!delimiters) return { { &string, 1 }, Error() };

    size_t wordsCount = _countWords(string, delimiters, length);
    if (wordsCount == 1) return { { &string, 1 }, Error() };

    String* words = (String*)calloc(wordsCount, sizeof(*words));

    char* currentToken = strtok(string.buf, delimiters);
    size_t i = 0;
    while (currentToken)
    {
        words[i].Create(currentToken);
        currentToken = strtok(nullptr, delimiters);
        i++;
    }

    return { words, i };
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

String::String() noexcept
    : buf(nullptr), capacity(0), length(0), error()
{
    error = this->Create();
}

String::String(size_t capacity) noexcept
    : buf(nullptr), capacity(0), length(0), error()
{
    error = this->Create(capacity);
}

String::String(const char* string) noexcept
    : buf(nullptr), capacity(0), length(0), error()
{
    error = this->Create(string);
}

String::String(const char* string, size_t length) noexcept
    : buf(nullptr), capacity(0), length(0), error()
{

    error = this->Create(string, length);
}

String::String(const String& other) noexcept
    : buf(nullptr), capacity(0), length(0), error()
{
    error = this->Create(other);
}

String::String(String&& other) noexcept
    : buf(other.buf), capacity(other.capacity),
      length(other.length), error(other.error)
{
    other.buf      = nullptr;
    other.capacity = SIZET_POISON;
    other.length   = SIZET_POISON;
}

String::~String() noexcept
{
    this->Destructor();
}

String& String::operator+=(const String& other) noexcept
{
    error = this->Append(other);
    return *this;
}

String& String::operator=(const String& other) noexcept
{
    error = this->Create(other);
    return *this;
}

String& String::operator=(String&& other) noexcept
{
    this->buf      = other.buf;
    this->capacity = other.capacity;
    this->length   = other.length;

    other.buf      = nullptr;
    other.capacity = SIZET_POISON;
    other.length   = SIZET_POISON;

    return *this;
}

String operator+(const String& left, const String& right) noexcept
{
    return left.Concat(right);
}
