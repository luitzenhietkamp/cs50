#include <cs50.h>
#include <stdio.h>

int main(void)
{
    float change;

    // ask the user for input
    do
    {
        change = get_float("Enter the amount of change owed: $");
    }
    while (change < 0);

    // convert the amount to cents (float to int)
    int cents = 100 * change;
    // but make sure that fractional sets are rounded, not truncated
    if ((100 * change - cents) >= 0.005)
    {
        ++cents;
    }

    // calculate the quarters
    int coins = cents / 25;
    cents -= 25 * coins;

    // calculate the dimes
    coins += cents / 10;
    cents -= 10 * (cents / 10);

    // calculate the nickels
    coins += cents / 5;

    // calculate the pennies
    coins += cents % 5;

    // produce the output
    printf("%i\n", coins);
}
