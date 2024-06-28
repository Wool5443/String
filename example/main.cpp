#include <cassert>
#include "String.hpp"

int main()
{
    using namespace Containers;

    String a{"Hello"};
    std::cout << a << '\n';

    String b = a;
    std::cout << a << '\n';
    std::cout << b << '\n';

    String c = a;
    c += b;

    std::cout << c << '\n';

    String t1{"hello ! friend!"};
    String t2{"bye bye sigma nice job"};

    std::cout << '\n';

    std::cout << t1 << '\n';
    std::cout << t2 << '\n';

    std::cout << '\n';

    Buffers::Array<String> words1 = t1.Split();
    Buffers::Array<String> words2 = t2.Split();

    for (size_t i = 0, end = words1.Length(); i < end; i++)
        std::cout << "words1[" << i << "]" << " = " << words1[i] << ", ";
    std::cout << "\n\n";
    for (size_t i = 0, end = words2.Length(); i < end; i++)
        std::cout << "words1[" << i << "]" << " = " << words2[i] << ", ";
    std::cout << "\n";

    return 0;
}
