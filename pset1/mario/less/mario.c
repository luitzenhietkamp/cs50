#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int n;

    // first ask user for input
    do
    {
        n = get_int("Height: ");
    }
    while (n < 0 || n > 23);

    // use input to build pyramid, line by line
    for (int i = 0; i < n; ++i)
    {
        // print required amount of spaces
        for (int j = 0; j < n - i - 1; ++j)
        {
            printf(" ");
        }
        // print block for left half of pyramid
        for (int j = 0; j < i + 2; ++j)
        {
            printf("#");
        }
        printf("\n");
    }
}
