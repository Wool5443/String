#pragma once

#include "Utils.hpp"

struct String;
struct StringResult;
struct SplitString;
struct SplitStringResult;

struct String
{
    char*  buf;
    size_t capacity;
    size_t length;
    Error  error;

    String() noexcept;
    String(size_t capacity) noexcept;
    String(const char* string) noexcept;
    String(const char* string, size_t length) noexcept;
    ~String() noexcept;

    String(const String& other) noexcept;

    String& operator+=(const String& other) noexcept;
    String& operator=(const String& other) noexcept;
    String& operator=(String&& other) noexcept;

    Error             Create() noexcept;
    Error             Create(size_t capacity) noexcept;
    Error             Create(const char* string) noexcept;
    Error             Create(const char* string, size_t length) noexcept;
    Error             Create(const String& string) noexcept;
    void              Destructor() noexcept;

    Error             Append(char chr) noexcept;
    Error             Append(const char*   string) noexcept;
    Error             Append(const String& string) noexcept;

    StringResult      Concat(const char*   string) const noexcept;
    StringResult      Concat(const String& string) const noexcept;

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

StringResult operator+(const String& left, const String& right) noexcept;
