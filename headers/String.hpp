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

    Error             Create() noexcept;
    Error             Create(size_t capacity) noexcept;
    Error             Create(const char* string) noexcept;
    Error             Create(const char* string, size_t length) noexcept;
    Error             Create(const String& string) noexcept;
    void              Destructor() noexcept;

    Error             Append(char chr) noexcept;
    Error             Append(const char*   string) noexcept;
    Error             Append(const String& string) noexcept;

    StringResult      Concat(const char*   string) noexcept;
    StringResult      Concat(const String& string) noexcept;

    size_t            Count(char character) const noexcept;
    size_t            Count(const char*   string) const noexcept;
    size_t            Count(const String& string) const noexcept;

    SplitStringResult Split() noexcept;
    SplitStringResult Split(const char*   delimiters) noexcept;
    SplitStringResult Split(const String& delimiters) noexcept;

    Error             Filter() noexcept;
    Error             Filter(const char*   filter) noexcept;
    Error             Filter(const String& filter) noexcept;

    bool              IsSpaceCharacters() const noexcept;
    static bool       IsSpaceCharacters(const char* string) noexcept;
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
