#include <errno.h>
#include "String.hpp"
#include "StringSettings.hpp"

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

ErrorCode _create(String* string, size_t capacity)
{
    MyAssertSoft(string, ERROR_NULLPTR);

    string->buf = (char*)calloc(capacity, 1);
    if (!string->buf) return ERROR_NO_MEMORY;

    string->length    = 0;
    string->capacity  = capacity;
    string->allocated = true;

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

StringResult _concat(const String* string, const char* add, size_t length)
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
