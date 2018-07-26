#include <cs50.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    // error message when incorrect number of command line arguments
    if (argc != 2)
    {
        printf("Error, incorrect number of command line arguments.\n");
        return 1;
    }

    int secret_number = 0;

    // argv[1] returns a pointer to char
    // meaning that argv[1] points to a null terminated
    // character array
    // we need to convert this pointer to an int
    for (int i = 0; argv[1][i] != '\0'; ++i)
    {
        secret_number *= 10;
        // the number is coded in ASCII
        // number 1 is coded as 49 in ASCII
        // in order to correctly get 1, we need
        // to deduct argv[1][i] by 48
        secret_number += argv[1][i] - 48;
    }

    // secret_number 1 will have the same effect as 27
    secret_number %= 26;

    // Ask the user for a message to encode
    string message;
    do
    {
        message = get_string("Please provide a message you wish to encode: ");
    }
    while (strlen(message) == 0);

    size_t char_num = strlen(message);
    for (size_t i = 0; i < char_num; ++i)
    {
        if (message[i] >= 'A' && message[i] <= 'Z')
        {
            message[i] = (message[i] + secret_number - 'A') % 26 + 'A';
        }
        else if (message[i] >= 'a' && message[i] <= 'z')
        {
            message[i] = (message[i] + secret_number - 'a') % 26 + 'a';
        }
    }

    printf("ciphertext: %s\n", message);
}
