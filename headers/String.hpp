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
                      String(char* buf, size_t capacity, size_t length, Error error);
                      ~String() noexcept;

                      String(const String& other) noexcept;
                      String(String&& other) noexcept;

                      operator char*() { return this->buf; };

    String&           operator+=(const char* other) noexcept;
    String&           operator+=(const String& other) noexcept;
    String&           operator=(const String& other) noexcept;
    String&           operator=(String&& other) noexcept;


    Error             Append(char chr) noexcept;
    Error             Append(const char*   string) noexcept;
    Error             Append(const String& string) noexcept;

    String            Concat(const char*   string) const noexcept;
    String            Concat(const String& string) const noexcept;

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

struct SplitString
{
    String* words;
    size_t  wordsCount;

    SplitString() noexcept;
    SplitString(const SplitString& other) = delete;
    SplitString(SplitString&& other) = delete;
    SplitString(String* words, size_t wordsCount) noexcept;
    ~SplitString() noexcept;

    String& operator=(const SplitString& other) = delete;
    String& operator=(SplitString&& other) = delete;

    void    Destructor();
};

struct SplitStringResult
{
    SplitString value;
    Error       error;
};

String operator+(const String& left, const String& right) noexcept;

static const String BAD_STRING(nullptr, SIZET_POISON, SIZET_POISON, Error());
