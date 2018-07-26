// program that verifies a credit card
#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // first get our input
    long long int input;
    do
    {
        input = get_long_long("Enter your credit card number: ");
    }
    while (input < 0);

    // set up some variables
    int sum = 0;
    bool times_2 = false;
    int company;
    int length = 0;

    // analyze the credit card number one digit at a time
    while (input > 0)
    {
        int n;
        ++length;
        // store last digit in n
        n = input % 10;
        // multiply by 2 if needed
        n *= (1 + times_2);

        // remove last digit from input
        input /= 10;

        // add the correct number to sum
        if (n < 10)
        {
            sum += n;
        }
        else
        {
            sum += (n % 10) + 1;
        }

        // change whether n needs to be
        // multiplied by 2 or 1 next time
        times_2 = !times_2;

        // when appropriate, store the company
        // identifier in company
        if (input < 100 && input > 10)
        {
            company = input;
        }
    }

    // make sure that an invalid input will
    // lead to the appropriate output
    if (sum % 10 != 0)
    {
        company = 0;
    }

    // print the correct output
    if ((company == 31 || company == 37) && length == 15)
    {
        printf("AMEX\n");
    }
    else if (company > 50 && company < 56 && length == 16)
    {
        printf("MASTERCARD\n");
    }
    else if (company > 39 && company < 50 && (length == 13 || length == 16))
    {
        printf("VISA\n");
    }
    else
    {
        printf("INVALID\n");
    }
}
