#pragma once

#include "Utils.hpp"

struct String;
struct StringResult;
struct SplitString;
struct SplitStringResult;
struct String
{
    char*  buf;
    size_t length;
    size_t capacity;
    bool   allocated;

    ErrorCode         Create();
    ErrorCode         Create(size_t        capacity);
    ErrorCode         Create(char*         string);
    ErrorCode         Create(const String* string);
    void              Destructor();

    ErrorCode         Append(char chr);
    ErrorCode         Append(const char*   string);
    ErrorCode         Append(const String* string);

    StringResult      Concat(const char*   string);
    StringResult      Concat(const String* string);

    size_t            Count(char character);
    size_t            Count(const char*   string);
    size_t            Count(const String* string);

    SplitStringResult Split();
    SplitStringResult Split(const char*   delimiters);
    SplitStringResult Split(const String* delimiters);
};

struct StringResult
{
    String    value;
    ErrorCode error;
};

struct SplitString
{
    String* words;
    size_t  wordsCount;

    void    Destructor();
};

struct SplitStringResult
{
    SplitString value;
    ErrorCode   error;
};
