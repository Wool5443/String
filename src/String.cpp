#include <errno.h>
#include <math.h>
#include <ctype.h>
#include "String.hpp"
#include "StringSettings.hpp"

static const char*  SPACE_CHARS        = " \t\n\r\v\f";
static const size_t SPACE_CHARS_LENGTH = 6;

static ErrorCode         _createImmutable(String* string, const char* buffer, size_t length);
static ErrorCode         _createMutable(String* string, size_t capacity);
static size_t            _countStr(const char*   string, const char* needle);
static StringResult      _concat(const String* string, const char* add, size_t length);
static SplitStringResult _split(String* string, const char* delimiters, size_t length);
static size_t            _countWords(const String* string, const char* delimiters, size_t length);
ErrorCode                _append(String* string, const char* add, size_t length);
static ErrorCode         _realloc(String* string, size_t neededLength);
static ErrorCode         _filter(String* string, const char* filter);

ErrorCode String::Create()
{
    return _createMutable(this, DEFAULT_STRING_CAPACITY);
}

ErrorCode String::Create(size_t capacity)
{
    return _createMutable(this, capacity);
}

ErrorCode String::Create(const char* string)
{
    MyAssertSoft(string, ERROR_NULLPTR);

    return _createImmutable(this, string, strlen(string));
}

ErrorCode String::Create(const char* string, size_t length)
{
    MyAssertSoft(string, ERROR_NULLPTR);

    return _createImmutable(this, string, length);
}

ErrorCode String::Create(const String* string)
{
    MyAssertSoft(string, ERROR_NULLPTR);

    if (this->allocated) free(this->buf);

    if (!string) return _createMutable(this, DEFAULT_STRING_CAPACITY);

    this->buf       = strdup(string->buf);
    if (!this->buf) return ERROR_NO_MEMORY;

    this->length    = string->length;
    this->capacity  = string->capacity;
    this->allocated = true;

    return EVERYTHING_FINE;
}

static ErrorCode _createImmutable(String* string, const char* buffer, size_t length)
{
    MyAssertSoft(string, ERROR_NULLPTR);
    MyAssertSoft(buffer, ERROR_NULLPTR);

    if (string->allocated && string->buf) free(string->buf);

    string->allocated = false;
    string->buf       = (char*)buffer;
    string->capacity  = length + 1;
    string->length    = length;

    return EVERYTHING_FINE;
}

static ErrorCode _createMutable(String* string, size_t capacity)
{
    MyAssertSoft(string, ERROR_NULLPTR);

    string->buf = (char*)calloc(capacity, 1);
    if (!string->buf) return ERROR_NO_MEMORY;

    string->length    = 0;
    string->capacity  = capacity;
    string->allocated = true;

    return EVERYTHING_FINE;
}

void String::Destructor()
{
    if (this->allocated) free(this->buf);
    this->allocated = false;
    this->buf       = nullptr;
    this->capacity  = 0;
    this->length    = 0;
}

void SplitString::Destructor()
{
    free(this->words);
    this->words      = nullptr;
    this->wordsCount = 0;
}

ErrorCode String::Append(char chr)
{
    if (!this->allocated) return ERROR_NOT_OWNER;

    RETURN_ERROR(_realloc(this, this->length + 1));

    this->buf[this->length++] = chr;

    return EVERYTHING_FINE;
}

ErrorCode String::Append(const char* string)
{
    return _append(this, string, strlen(string));
}

ErrorCode String::Append(const String* string)
{
    return _append(this, string->buf, string->length);
}

ErrorCode _append(String* string, const char* add, size_t length)
{
    MyAssertSoft(string, ERROR_NULLPTR);
    MyAssertSoft(add,    ERROR_NULLPTR);

    if (!string->allocated) return ERROR_NOT_OWNER;

    RETURN_ERROR(_realloc(string, string->length + length));

    strncpy(string->buf + string->length, add, length);
    string->length += length;

    return EVERYTHING_FINE;
}

StringResult String::Concat(const char* string)
{
    MyAssertSoftResult(string, {}, ERROR_NULLPTR);

    return _concat(this, string, strlen(string));
}

StringResult String::Concat(const String* string)
{
    MyAssertSoftResult(string, {}, ERROR_NULLPTR);

    return _concat(this, string->buf, string->length);
}

static StringResult _concat(const String* string, const char* add, size_t length)
{
    MyAssertSoftResult(string, {}, ERROR_NULLPTR);
    MyAssertSoftResult(add,    {}, ERROR_NULLPTR);

    String newString = {};
    size_t newLength = string->length + length;

    newString.capacity  = DEFAULT_STRING_CAPACITY;
    newString.allocated = true;
    ErrorCode error     = _realloc(&newString, newLength);
    if (error) return { {}, error };

    strncpy(newString.buf, string->buf, string->length);
    strncpy(newString.buf + string->length, add, length);

    newString.length = newLength;

    return { newString, EVERYTHING_FINE };
}

size_t String::Count(char character)
{
    size_t count = 0;
    for (size_t i = 0; i < this->length; i++)
        if (this->buf[i] == character)
            count++;

    return count;
}

size_t String::Count(const char* string)
{
    return _countStr(this->buf, string);
}

size_t String::Count(const String* string)
{
    return _countStr(this->buf, string->buf);
}

static size_t _countStr(const char* string, const char* needle)
{
    MyAssertSoft(string, ERROR_NULLPTR);
    MyAssertSoft(needle, ERROR_NULLPTR);

    size_t      count = 0;
    const char* found = strstr(string, needle);

    while (found)
    {
        count++;
        found = strstr(found, needle);
    }

    return count;
}

SplitStringResult String::Split()
{
    return _split(this, SPACE_CHARS, SPACE_CHARS_LENGTH);
}

SplitStringResult String::Split(const char* delimiters)
{
    return _split(this, delimiters, strlen(delimiters));
}

SplitStringResult String::Split(const String* delimiters)
{
    return _split(this, delimiters->buf, delimiters->length);
}

static SplitStringResult _split(String* string, const char* delimiters, size_t length)
{
    MyAssertSoftResult(string, {}, ERROR_NULLPTR);
    
    if (!delimiters) return { { (String*)string, 1 }, EVERYTHING_FINE };

    size_t wordsCount  = _countWords(string, delimiters, length);
    if (wordsCount == 1) return { { (String*)string, 1 }, EVERYTHING_FINE };

    String* words = (String*)calloc(wordsCount, sizeof(*words));

    char* currentToken = strtok(string->buf, delimiters);
    size_t i = 0;
    while (currentToken)
    {
        words[i].Create(currentToken);
        currentToken = strtok(nullptr, delimiters);
        i++;
    }

    return { words, i };
}

static size_t _countWords(const String* string, const char* delimiters, size_t length)
{
    MyAssertSoft(string,     ERROR_NULLPTR);
    MyAssertSoft(delimiters, ERROR_NULLPTR);

    size_t wordsCount = 1;

    for (size_t i = 0; i < string->length; i++)
    {
        for (size_t j = 0; j < length; j++)
        {
            if (string->buf[i] == delimiters[j])
            {
                wordsCount++;
                break;
            }
        }
    }

    return wordsCount;
}

static ErrorCode _realloc(String* string, size_t neededLength)
{
    MyAssertSoft(string, ERROR_NULLPTR);
    MyAssertSoft(string->allocated, ERROR_NOT_OWNER);

    if (neededLength < string->capacity)
        return EVERYTHING_FINE;

    size_t newCapacity  = string->capacity * 
                          (size_t)pow(2, (size_t)log2((double)neededLength / (double)string->capacity) + 1);

    char* newBuf = (char*)calloc(newCapacity, 1);
    if (!newBuf) return ERROR_NO_MEMORY;
    if (string->buf)
        memcpy(newBuf, string->buf, string->capacity);

    free(string->buf);
    string->buf      = newBuf;
    string->capacity = newCapacity;

    return EVERYTHING_FINE;
}

ErrorCode String::Filter()
{
    return _filter(this, SPACE_CHARS);
}

ErrorCode String::Filter(const char* filter)
{
    MyAssertSoft(filter, ERROR_NULLPTR);
    return _filter(this, filter);
}

ErrorCode String::Filter(const String* filter)
{
    MyAssertSoft(filter, ERROR_NULLPTR);
    return _filter(this, filter->buf);
}

static ErrorCode _filter(String* string, const char* filter)
{
    MyAssertSoft(string, ERROR_NULLPTR);
    MyAssertSoft(filter, ERROR_NULLPTR);

    const char* readPtr  = string->buf;
    char*       writePtr = string->buf;

    while (*readPtr)
    {
        char c = *readPtr++;

        if (!strchr(filter, c))
            *writePtr++ = c;
    }
    *writePtr = '\0';

    return EVERYTHING_FINE;
}

bool String::IsSpaceCharacters()
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
