#include "String.hpp"

int main()
{
    String a = (String)"hello" + "world";

    puts(a.buf);

    return 0;
}
