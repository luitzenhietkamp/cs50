import sys
from cs50 import get_string

# ensure proper usage
if not len(sys.argv) == 2:
    print("Usage: python caesar.py k")
    sys.exit(1)

key = int(sys.argv[1])

# prompt user for input
message = get_string("plaintext: ")

# encrypt the message
ciphertext = ""
for c in message:
    if c.isupper():
        ciphertext += chr((ord(c) - ord("A") + key) % 26 + ord("A"))
    elif c.islower():
        ciphertext += chr((ord(c) - ord("a") + key) % 26 + ord("a"))
    else:
        ciphertext += c

# output result
print("ciphertext: ", end="")
print(ciphertext)