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

    Error Create();
    Error Create(size_t capacity);
    Error Create(const char* string);
    Error Create(const char* string, size_t length);
    Error Create(const String* string);
    void              Destructor();

    Error Append(char chr);
    Error Append(const char*   string);
    Error Append(const String* string);

    StringResult      Concat(const char*   string);
    StringResult      Concat(const String* string);

    size_t            Count(char character) const;
    size_t            Count(const char*   string) const;
    size_t            Count(const String* string) const;

    SplitStringResult Split();
    SplitStringResult Split(const char*   delimiters);
    SplitStringResult Split(const String* delimiters);

    Error Filter();
    Error Filter(const char*   filter);
    Error Filter(const String* filter);

    bool              IsSpaceCharacters() const;

    static bool       IsSpaceCharacters(const char* string);
};

struct StringResult
{
    String value;
    Error  error;
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
    Error       error;
};
