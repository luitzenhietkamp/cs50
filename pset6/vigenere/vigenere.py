import sys
from cs50 import get_string

# ensure proper usage
if not len(sys.argv) == 2 or not sys.argv[1].isalpha():
    print("Usage python vigenere.py k")
    sys.exit(1)
else:
    key = sys.argv[1].upper()

# prompt user for input
message = get_string("plaintext: ")

# encrypt the message
ciphertext = ""
i = 0
for c in message:
    if c.isupper():
        ciphertext += chr((ord(c) + ord(key[i]) - 2 * ord("A")) % 26 + ord("A"))
        i += 1
    elif c.islower():
        ciphertext += chr((ord(c) + ord(key[i]) - ord("A") - ord("a")) % 26 + ord("a"))
        i += 1
    else:
        ciphertext += c
    if len(sys.argv[1]) == i:
        i = 0

# output the result
print("ciphertext: " + ciphertext)