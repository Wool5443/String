#include "String.hpp"

int main()
{
    Containers::String a{"Hello"};
    std::puts(a);

    Containers::String b = a;
    std::puts(a);
    std::puts(b);

    Containers::String c = a;
    c += b;

    std::puts(c);

    return 0;
}
