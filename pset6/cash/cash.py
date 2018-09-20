from cs50 import get_float

# prompt for user input
while True:
    change = 100 * get_float("Change owed: ")
    if not change < 0:
        break

# calculate quarters...
coins = change // 25
change = change % 25

# dimes...
coins += change // 10
change = change % 10

# nickles...
coins += change // 5
change = change % 5

# ... and pennies
coins += change // 1

# output result
print(coins)