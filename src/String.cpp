#include <errno.h>
#include "String.hpp"
#include "StringSettings.hpp"

static const char*  SPACE_CHARS        = " \t\n\r\v\f";
static const size_t SPACE_CHARS_LENGTH = 6;

static ErrorCode         _create  (String* string,       size_t capacity);
static size_t            _countStr(const char*   string,   const char* needle);
static StringResult      _concat  (const String* string, const char* add, size_t length);
static SplitStringResult _split   (String* string, const char* delimiters, size_t length);
static size_t _countWords(const String* string, const char* delimiters, size_t length);

ErrorCode String::Create()
{
    return _create(this, DEFAULT_STRING_CAPACITY);
}

ErrorCode String::Create(size_t capacity)
{
    return _create(this, capacity);
}

ErrorCode String::Create(char* string)
{
    MyAssertSoft(string, ERROR_NULLPTR);

    if (this->allocated && this->buf) free(this->buf);

    if (!string) return _create(this, DEFAULT_STRING_CAPACITY);

    this->buf       = string;
    this->length    = strlen(string);
    this->capacity  = this->length;
    this->allocated = false;

    return EVERYTHING_FINE;
}

ErrorCode String::Create(const String* string)
{
    MyAssertSoft(string, ERROR_NULLPTR);

    if (this->allocated && this->buf) free(this->buf);

    if (!string) return _create(this, DEFAULT_STRING_CAPACITY);

    this->buf       = strdup(string->buf);
    if (!this->buf) return ERROR_NO_MEMORY;

    this->length    = string->length;
    this->capacity  = string->capacity + 1;
    this->allocated = true;

    return EVERYTHING_FINE;
}

static ErrorCode _create(String* string, size_t capacity)
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

    ErrorCode error  = newString.Create(newLength + 1);
    if (error) return { {}, error };

    strncpy(newString.buf, string->buf, string->length);
    strncat(newString.buf, add, length);

    newString.length = string->length + length;

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

    size_t wordsCount = 0;

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
