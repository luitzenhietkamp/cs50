from cs50 import get_int

# prompt for user input
while True:
    n = get_int("Height: ")
    if n >= 0 and n < 24:
        break

# print the pyramid
for i in range(n):
    for j in range(n - i - 1):
        print(" ", end="")
    for j in range(i + 2):
        print("#", end="")
    print()