#include "String.hpp"

int main()
{
    char text[] = "hello friends. world, amigo!!!\tverbal";

    String string = {};
    string.Create(text);

    SplitStringResult split = string.Split(" .,!\t\n");

    if (split.error)
    {
        printf("%s[%d]\n", ERROR_CODE_NAMES[split.error], split.error);
        return split.error;
    }

    for (size_t i = 0; i < split.value.wordsCount; i++)
        printf("%s\n", split.value.words[i].buf);

    string.Destructor();
    split.value.Destructor();

    return 0;
}