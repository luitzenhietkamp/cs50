#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    // error message when incorrect number of command line arguments
    if (argc != 2)
    {
        printf("Error: incorrect number of command line arguments.\n");
        return 1;
    }

    // put the command line argument in a string
    string keyword = argv[1];

    // error message when incorrect command line argument
    for (int i = 0; keyword[i] != '\0'; ++i)
    {
        if (!isalpha(keyword[i]))
        {
            printf("Error: incorrect command line argument detected.\n");
            printf("The key should only contain alphabetic characters.\n");
            return 1;
        }
    }

    // Ask the user for a message to encode
    string message;
    do
    {
        message = get_string("Please provide a message you wish to encode: ");
    }
    while (strlen(message) == 0);

    // create variables for the length of the strings
    size_t char_num = strlen(message);
    size_t key_size = strlen(keyword);

    // encrypt the message
    for (size_t i = 0, j = 0; i < char_num; ++i)
    {
        int secret_number = isupper(keyword[j]) ? keyword[j] - 'A' : keyword[j] - 'a';
        char c = message[i];

        // encrypt the car in case
        if (isupper(c))
        {
            message[i] = (c + secret_number - 'A') % 26 + 'A';
        }
        else if (islower(c))
        {
            message[i] = (c + secret_number - 'a') % 26 + 'a';
        }

        // increase j if c is a letter..
        if (isalpha(c))
        {
            // if we arive past the end of keyword..
            if (++j == key_size)
            {
                // ..set j to the beginning of keyword
                j = 0;
            }
        } // otherwise keep j the same
    }

    printf("ciphertext: %s\n", message);
}
