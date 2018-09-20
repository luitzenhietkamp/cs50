from cs50 import get_int

# prompt user for input
n = get_int("Number: ")

digits = len(str(n))
company = "INVALID"

# add up the digits
sum_n = 0
i = 0
while True:
    digit = (n % 10) * 2 ** (i % 2)
    i += 1
    sum_n += digit // 10 + digit % 10
    n //= 10
    # stop when all digits have been processed
    if n <= 1:
        break
    # choose correct company
    if n == 34 or n == 37 and digits == 15:
        company = "AMEX"
    if n > 50 and n < 56 and digits == 16:
        company = "MASTERCARD"
    if n == 4 and (digits == 13 or digits == 16):
        company = "VISA"

# validate the sum
if not sum_n % 10 == 0:
    company = "INVALID"

# output result
print(company)