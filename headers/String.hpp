#pragma once

#include "Utils.hpp"
#include "StringBuffer.hpp"

namespace String
{
class String
{
private:
    char*             m_buf;
    std::size_t       m_capacity;
    std::size_t       m_length;
    Utils::Error      m_error;
public:
                      String()                                       noexcept;
                      String(std::size_t capacity)                   noexcept;
                      String(const char* string)                     noexcept;
                      String(const char* string, std::size_t length) noexcept;
                      String(const String& other)                    noexcept;
                      String(String&& other)                         noexcept;
                      ~String()                                      noexcept;

    String&           operator+=(const char* other)                  noexcept;
    String&           operator+=(const String& other)                noexcept;
    String&           operator=(const String& other)                 noexcept;
    String&           operator=(String&& other)                      noexcept;

    std::size_t       Count(char character)                    const noexcept;
    std::size_t       Count(const char*   string)              const noexcept;
    std::size_t       Count(const String& string)              const noexcept;

    Utils::Error      Filter()                                       noexcept;
    Utils::Error      Filter(const char*   filter)                   noexcept;
    Utils::Error      Filter(const String& filter)                   noexcept;

    // SplitStringResult Split() noexcept;
    // SplitStringResult Split(const char*   delimiters)   noexcept;
    // SplitStringResult Split(const String& delimiters)   noexcept;

    bool              IsSpaceCharacters() const noexcept;
    static bool       IsSpaceCharacters(const char* string);
};

// struct SplitString
// {
//     String* words;
//     std::size_t  wordsCount;

//     SplitString() noexcept;
//     SplitString(const SplitString& other) = delete;
//     SplitString(SplitString&& other) = delete;
//     SplitString(String* words, std::size_t wordsCount) noexcept;
//     ~SplitString() noexcept;

//     String& operator=(const SplitString& other) = delete;
//     String& operator=(SplitString&& other) = delete;

//     void    Destructor();
// };

// struct SplitStringResult
// {
//     SplitString value;
//     Error       error;
// };

String operator+(const String& left, const String& right) noexcept;
}
