#pragma once

#include "StringBuffer.hpp"

namespace Containers
{
<<<<<<< HEAD
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
=======
class String
>>>>>>> Actually-good-string
{
private:
    Buffers::StringBuffer m_buf;
    String(std::size_t capacity, std::size_t length);
public:
    String();
    String(std::size_t capacity);
    String(const char* string);
    String(const char* string, std::size_t length);

    inline char*        __attribute__((always_inline)) Buffer() { return m_buf.Buffer(); }
    inline std::size_t  __attribute__((always_inline)) Length() { return m_buf.Length(); }
    inline Utils::Error __attribute__((always_inline)) Error()  { return m_buf.Error(); }

    operator Utils::Error() { return this->Error(); }
    operator char*()        { return this->Buffer(); }
    operator bool()         { return this->Buffer(); }
};
}