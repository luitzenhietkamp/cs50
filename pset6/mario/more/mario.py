from cs50 import get_int

# prompt for user input
while True:
    n = get_int("Height: ")
    if n >= 0 and n < 24:
        break

# print the pyramid
for i in range(n):
    print(" " * (n - i - 1), end="")
    print("#" * (i + 1), end="")
    print("  ", end="")
    print("#" * (i + 1))