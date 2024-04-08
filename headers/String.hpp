#pragma once

#include "Utils.hpp"

struct StringResult;

struct String
{
    char*  buf;
    size_t length;
    size_t capacity;
    bool   allocated;

    ErrorCode    Create();
    ErrorCode    Create(size_t capacity);
    ErrorCode    Create(char*   string);
    ErrorCode    Create(const String* string);
    ErrorCode    Destructor();

    StringResult Concat(const char*   string);
    StringResult Concat(const String* string);
};

struct StringResult
{
    String    value;
    ErrorCode error;
};
