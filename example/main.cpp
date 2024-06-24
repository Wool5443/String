#include "String.hpp"

int main()
{
    String a("Hello");
    a += " ";
    a += "world";
    String b = a;

    a.buf[0] = 'h';

    puts(a.buf);
    puts(b.buf);

    b = "Hello my dear friends. Today we are going to test some strings.";

    SplitStringResult wordsRes = b.Split();

    if (wordsRes.error)
    {
        wordsRes.error.Print();
        return wordsRes.error;
    }

    for (size_t i = 0; i < wordsRes.value.wordsCount; i++)
        puts(wordsRes.value.words[i].buf);

    return 0;
}
