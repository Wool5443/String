#include <cassert>
#include "String.hpp"

int main()
{
    using namespace Containers;

    String a{"Hello"};
    std::puts(a);

    String b = a;
    std::puts(a);
    std::puts(b);

    String c = a;
    c += b;

    std::puts(c);

    String t1{"hello ! friend!"};
    String t2{"bye bye sigma nice job"};

    return 0;
}
