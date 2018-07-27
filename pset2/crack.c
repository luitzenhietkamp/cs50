#define _XOPEN_SOURCE

#include <cs50.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// function that checks if string a matches with hash b
bool hashes_match(char a[], char b[], char salt[])
{
    return !strcmp(crypt(a, salt), b);
}

int main(int argc, char *argv[])
{
    // error message when incorrect number of command line arguments
    if (argc != 2)
    {
        printf("Usage: ./crack hash\n");
        return 1;
    }

    // strategy:
    // brute force hash character arrays and check whether the hashes match
    // if the hashes are matching, passwords are matching
    // or at least the alternative password could be used to log in

    // potentially slow
    // doesn't appear to be realistic
    // saving grace: passwords are max 5 characters long

    // alternative: only hash words in the English dictionary
    // that are at max 5 characters long
    char test[] = "AAAAA";

    // create the salt
    char salt[2];
    salt[0] = argv[1][0];
    salt[1] = argv[1][1];

    // for each string length i
    for (int i = 0; i < 5; ++i)
    {
        // setup string
        // doing permutations on strings is like base-52 arithmetic,
        // but a bit more complicated
        // in order to be able to do base-52 arithmetic on strings
        // we need to set up the string equivalent of 0000..00
        // where 0 = A
        // we will also write our base 52 numbers from left to right
        for (int j = 0; j < i + 1; ++j)
        {
            test[j] = 'A';
        }
        test[i + 1] = '\0';

        // check if base-52 versions of 0, 00, 000, ... have a hash
        // that matches with the input
        if (hashes_match(test, argv[1], salt))
        {
            printf("%s\n", test);
            return 0;
        }
        // we'll count to the base-52 versions of 10, 100, 1000, ...
        for (int j = 0, power = pow(52, 1 + i); j < power; ++j)
        {
            // adder is the digit where we add 1
            int adder = 0;
            bool has_added = false;
            while (!has_added)
            {
                // if a number is 9, we set it to 0
                // and then add 1 to the next digit
                if (test[adder] == 'z')
                {
                    test[adder] = 'A';
                    adder++;
                }
                else
                {
                    // if the digit is not 9 we can
                    // add 1 and we're done adding
                    has_added = true;
                    // but we need to take into
                    // account that a comes after Z
                    if (test[adder] == 'Z')
                    {
                        test[adder] = 'a';
                    }
                    else
                    {
                        ++test[adder];
                    }
                }
            }
            // check if the newly created base-52 number
            // has a matching hash
            if (hashes_match(test, argv[1], salt))
            {
                printf("%s\n", test);
                return 0;
            }
        }
    }
    printf("Password not found.\n");
}
